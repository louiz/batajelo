#include <logging/logging.hpp>
#include <game/game_client.hpp>
#include <utils/time.hpp>
#include <world/world_callbacks.hpp>
#include <world/task.hpp>

#include "orders.pb.h"
#include "requests.pb.h"

namespace ph = std::placeholders;

GameClient::GameClient(const std::shared_ptr<Screen>& screen):
  Game(),
  ClientBase(),
  current_selection(),
  screen(screen),
  camera(this, this->screen.get()),
  hud(this, this->screen.get()),
  debug_hud(this, this->screen.get())
{
  this->turn_handler.set_next_turn_callback(std::bind(&GameClient::on_next_turn,
                                               this, ph::_1));

  this->connect("127.0.0.1", 7879);

  this->install_callback(
      "OCCUPANT_LEFT",
      std::bind(&GameClient::occupant_left_callback, this, ph::_1));
  this->install_callback("SEED",
                         std::bind(&GameClient::handle_seed_message, this, ph::_1));
  this->install_callback("NEW_ENTITY",
                         std::bind(&Game::new_entity_callback, this, ph::_1));
  this->install_callback("START", std::bind(&GameClient::handle_start_message, this, ph::_1));
  this->install_callback("T", std::bind(&GameClient::turn_callback, this, ph::_1));
  this->install_callback("MOVE", std::bind(&Game::move_callback, this, ph::_1));
  this->install_callback("CAST", std::bind(&Game::cast_callback, this, ph::_1));

  this->screen->add_element(&this->camera, 0);
  this->screen->add_element(&this->hud, 1);
  this->screen->add_element(&this->get_debug_hud(), 2);

  /**
   * Install world callbacks
   */
  this->world.callbacks->entity_created =
      std::bind(&GameClient::on_entity_created, this, ph::_1);
  this->world.callbacks->task_changed =
      std::bind(&GameClient::on_entity_task_changed, this, ph::_1, ph::_2);
  this->world.callbacks->entity_deleted =
      std::bind(&GameClient::on_entity_deleted, this, ph::_1);
  this->world.callbacks->ability_casted =
      std::bind(&GameClient::on_ability_casted, this, ph::_1, ph::_2, ph::_3, ph::_4);
  this->world.callbacks->impact =
      std::bind(&GameClient::on_impact, this, ph::_1, ph::_2);

}

GameClient::~GameClient()
{
}

void GameClient::run()
{
  auto last_update = utils::now();

  // Dirty workaround, fixme. Should be 0.
  long ticks = -5;
  std::chrono::steady_clock::duration dt{0};

  // window->setMouseCursorVisible(false);
  while (this->screen->window().isOpen())
    {
      // Check for user events
      sf::Event event;
      while (this->screen->window().pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            this->screen->window().close();
          this->screen->handle_event(event);
        }

      // recv/send from the network
      this->poll();

      this->screen->window().clear(sf::Color(70, 80, 38));
      this->screen->draw();
      this->screen->window().display();

      // Get the elapsed time
      auto now = utils::now();
      auto elapsed = now - last_update;
      // Call update with the elapsed time
      this->screen->update(std::chrono::duration_cast<utils::Duration>(elapsed));

      // Update dt with the elapsed time. We add that to the remaining value
      // that was not “consumed” by a whole tick in the previous loop
      // iteration
      dt += elapsed;

      // Save the date of the last update
      last_update = now;

      // Tick everything, based on the elapsed time
      // this “consumes” dt. For example if the returned value is 3, dt is
      // reduced by 3 * tick_duration.
      ticks += utils::get_number_of_ticks(dt);
      for (; ticks > 0; --ticks)
        {
          if (!this->tick())
            break ;
          this->graphical_tick();
        }
    }
}

void GameClient::install_callbacks()
{
  this->install_callback(
      "NEW_OCCUPANT", std::bind(&GameClient::new_occupant_callback, this, ph::_1));
}

void GameClient::add_new_occupant(std::unique_ptr<Occupant>&& occupant)
{
  this->occupants_handler.add(std::move(occupant));
}

DebugHud& GameClient::get_debug_hud()
{
  return this->debug_hud;
}

Hud& GameClient::get_hud()
{
  return this->hud;
}

World& GameClient::get_world()
{
  return this->world;
}

Camera& GameClient::get_camera()
{
  return this->camera;
}

Screen& GameClient::get_screen()
{
  return *this->screen;
}

void GameClient::on_next_turn(TurnNb turn)
{
}

void GameClient::new_occupant_callback(Message* message)
{
  log_debug("new_occupant_callback");

  auto srl = message->parse_body_to_protobuf_object<ser::game::Occupant>();

  auto occupant = std::make_unique<Occupant>(srl);

  log_debug("Occupant: " << occupant->name << " " << occupant->id);
  if (this->occupants_handler.size() == 0)
    { // This occupant is actually us
      auto team = srl.team();
      this->set_self_team(team);
    }
  this->add_new_occupant(std::move(occupant));
}

void GameClient::occupant_left_callback(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::game::Occupant>();

  Occupant occupant(srl);
  log_debug("Occupant to remove: " << occupant.id);
  this->occupants_handler.remove(occupant);
}

void GameClient::handle_seed_message(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::order::Seed>();
  log_debug("Seed value received: " << srl.value());
  this->world.seed(srl.value());
}

void GameClient::handle_start_message(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::order::Start>();
  log_debug("The first turn to start is " << srl.turn());
  if (srl.turn() != 0)
    this->turn_handler.mark_as_ready_until(srl.turn());
  do
    {
      this->tick();
    } while (!this->turn_handler.is_paused());

  // this->world.start();
}

void GameClient::set_self_team(const uint16_t team)
{
  log_debug("Our team is number: " << team);
  this->team = team;
}

uint16_t GameClient::get_self_team() const
{
  return this->team;
}

void GameClient::turn_callback(Message*)
{
  log_debug("GameClient::turn_callback");
  this->turn_handler.mark_turn_as_ready();
}

void GameClient::path_callback(Message* message)
{
}

bool GameClient::action_move(const std::vector<EntityId>& ids, const Position& pos, const bool queue)
{
  ser::request::Move srl;
  if (queue)
    srl.set_queue(queue);
  srl.mutable_pos()->set_x(pos.x.raw());
  srl.mutable_pos()->set_y(pos.y.raw());
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  this->send_message("MOVE", srl);
  this->sounds_handler.play(SoundType::DefaultOk, true);
  return true;
}

bool GameClient::action_follow(const std::vector<EntityId>& ids,
                               const EntityId& target_id, const bool queue)
{
  ser::request::Move srl;
  if (queue)
    srl.set_queue(queue);
  srl.set_target(target_id);
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  this->send_message("MOVE", srl);
  this->sounds_handler.play(SoundType::DefaultOk, true);
  return true;
}

bool GameClient::action_cast(const std::vector<EntityId>& ids,
                             const Position& pos, const AbilityType& type,
                             const bool queue)
{
  ser::request::Cast srl;
  if (queue)
    srl.set_queue(queue);
  srl.mutable_pos()->set_x(pos.x.raw());
  srl.mutable_pos()->set_y(pos.y.raw());
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  srl.set_type(static_cast<uint32_t>(type));
  this->send_message("CAST", srl);
  return true;
}

bool GameClient::action_cast(const std::vector<EntityId>& ids,
                             const EntityId target_id, const AbilityType& type,
                             const bool queue)
{
  ser::request::Cast srl;
  if (queue)
    srl.set_queue(queue);
  srl.set_target(target_id);
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  srl.set_type(static_cast<uint32_t>(type));
  this->send_message("CAST", srl);
  return true;
}

bool GameClient::action_cast(const std::vector<EntityId>& ids,
                             const AbilityType& type, const bool queue)
{
  ser::request::Cast srl;
  if (queue)
    srl.set_queue(queue);
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  srl.set_type(static_cast<uint32_t>(type));
  this->send_message("CAST", srl);
  return true;
}

void GameClient::select_entity(const Entity* entity)
{
  this->current_selection.add_to_selection(entity);
}

void GameClient::unselect_entity(const Entity* entity)
{
  this->current_selection.remove_from_selection(entity);
}

void GameClient::clear_selection()
{
  this->current_selection.clear();
}

bool GameClient::is_entity_selected(const Entity* entity) const
{
  return this->current_selection.is_in_selection(entity);
}

Selection& GameClient::get_selection()
{
  return this->current_selection;
}

const Selection& GameClient::get_selection() const
{
  return this->current_selection;
}

void GameClient::add_selection_change_callback(const t_selection_changed_callback callback)
{
  this->current_selection.on_modified_callbacks.push_back(callback);
}

void GameClient::on_impact(const Entity* entity, const Entity* target)
{
  // Map of EntityType:SoundType?
  if (target)
    {
      this->sounds_handler.play(SoundType::ProjectileImpact, false, 20.f);
    }
}

void GameClient::on_entity_created(const Entity* entity)
{
  this->camera.on_new_entity(entity);
}

void GameClient::on_entity_task_changed(const Entity* entity, const Task* task)
{
  switch (task->get_type())
    {
      case TaskType::Concentrate:
        this->sounds_handler.play(SoundType::Concentrate, false, 100.f);
        break;
      case TaskType::Dash:
        this->sounds_handler.play(SoundType::Dash, false, 100.f);
        break;
    }
  this->camera.on_entity_task_changed(entity, task);
}

void GameClient::on_entity_deleted(const Entity* entity)
{
  this->camera.on_entity_deleted(entity);
  this->current_selection.remove_from_selection(entity);
  if (entity->get_type() == 2)
    this->sounds_handler.play(SoundType::EmpExplode, false, 100.f);
}

bool GameClient::tick()
{
  this->turn_handler.tick();
  if (this->turn_handler.is_paused())
    return false;
  this->world.tick();
  return true;
}
void GameClient::graphical_tick()
{
  this->camera.graphical_tick();
}

void GameClient::on_connection_closed()
{
  log_error("Connection closed by remote server");
}

void GameClient::on_ability_casted(const Entity* caster,
                                   const AbilityType& type,
                                   const Entity* target,
                                   const Position& position)
{
  log_debug("on_ability_casted: " << static_cast<std::size_t>(type));
  if (type == AbilityType::Blink)
    this->sounds_handler.play(SoundType::BlinkStart, false, 20.f);
  else if (type == AbilityType::Attack)
    this->sounds_handler.play(SoundType::ProjectileLaunch, false, 20.f);
  else if (type == AbilityType::Emp)
    this->sounds_handler.play(SoundType::EmpStart, false, 100.f);
}
