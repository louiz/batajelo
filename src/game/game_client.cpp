#include <logging/logging.hpp>
#include <game/game_client.hpp>
#include <game/time.hpp>

#include "orders.pb.h"
#include "requests.pb.h"

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
                                               this, std::placeholders::_1));

  this->connect("127.0.0.1", 7879);

  this->install_callback("OCCUPANT_LEFT",
                         std::bind(&GameClient::occupant_left_callback, this, std::placeholders::_1));
  this->install_callback("NEW_ENTITY",
                         std::bind(&Game::new_entity_callback, this, std::placeholders::_1));
  this->install_callback("START",
                         std::bind(&GameClient::handle_start_message, this, std::placeholders::_1));
  this->install_callback("T",
                         std::bind(&GameClient::turn_callback, this, std::placeholders::_1));
  this->install_callback("MOVE",
                         std::bind(&Game::move_callback, this, std::placeholders::_1));

  this->screen->add_element(&this->camera, 0);
  this->screen->add_element(&this->hud, 1);
  // this->screen->add_element(&this->get_debug_hud(), 2);
}

GameClient::~GameClient()
{
}

void GameClient::run()
{
  sf::Clock fps_clock;

  Time time1 = boost::posix_time::microsec_clock::universal_time();
  Time time2;
  Time graph_time1 = boost::posix_time::microsec_clock::universal_time();
  Time graph_time2;

  Duration dt;
  Duration graph_dt;

  // window->setMouseCursorVisible(false);
  while (this->screen->window().isOpen())
    {
      // Check for user events
      sf::Event event;
      while (this->screen->window().pollEvent(event))
        {
          if (event.type == sf::Event::Closed)
            {
              this->screen->window().close();
            }
          // if (event.type == sf::Event::GainedFocus)
          //   {
          //   }
          this->screen->handle_event(event);
        }

      // recv/send from the network
      this->poll(0);

      // Draw the screen. Limit to ~60 fps.
      if (fps_clock.getElapsedTime().asMicroseconds() > 10000)
        {
          this->screen->window().clear(sf::Color(70, 80, 38));
          this->screen->draw();
          this->screen->window().display();
          fps_clock.restart();
        }

      // Get the elapsed time
      time2 = boost::posix_time::microsec_clock::universal_time();
      dt += time2 - time1;
      time1 = time2;

      this->screen->update(dt);
      // Update everything, based on the elapsed time
      long i = get_number_of_updates(dt);

      for (; i > 0; --i)
        {
          this->tick();
        }

      graph_time2 = boost::posix_time::microsec_clock::universal_time();
      graph_dt += graph_time2 - graph_time1;
      graph_time1 = graph_time2;
      i = get_number_of_graphicale_updates(graph_dt);
      for (; i > 0; --i)
        {
          this->graphical_tick();
        }
    }
}

void GameClient::install_callbacks()
{
  this->install_callback("NEW_OCCUPANT",
                         std::bind(&GameClient::new_occupant_callback,
                                   this, std::placeholders::_1));
}

void GameClient::add_new_occupant(std::unique_ptr<Occupant>&& occupant)
{
  this->occupants_handler.add(std::move(occupant));
}

DebugHud& GameClient::get_debug_hud()
{
  return this->debug_hud;
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

void GameClient::send_message(const char* name, const google::protobuf::Message& msg)
{
  Message* message = new Message;
  message->set_name(name);
  message->set_body(msg);
  this->send(message);
}

void GameClient::send_message(const char* name, const std::string& archive)
{
  Message* message = new Message;
  message->set_name(name);
  message->set_body(archive.data(), archive.length());
  this->send(message);
}

void GameClient::on_next_turn(TurnNb turn)
{
}

void GameClient::new_occupant_callback(Message* message)
{
  log_debug("new_occupant_callback");

  auto srl = message->parse_body_to_protobuf_object<ser::game::Occupant>();
  if (!srl.IsInitialized())
    {
      log_error("Invalid data received for the occupant: " << srl.InitializationErrorString());
      return ;
    }

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
  if (!srl.IsInitialized())
    {
      log_error("Invalid data received for the leaving occupant." << srl.InitializationErrorString());
      return ;
    }

  Occupant occupant(srl);
  log_debug("Occupant to remove: " << occupant.id);
  this->occupants_handler.remove(occupant);
}

void GameClient::handle_start_message(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::order::Start>();
  if (!srl.IsInitialized())
    {
      log_warning("Invalid data for START message " << srl.InitializationErrorString());
      return ;
    }
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

bool GameClient::action_move(std::vector<EntityId> ids, const Position& pos, const bool queue)
{
  ser::request::Move srl;
  srl.set_queue(queue);
  srl.mutable_pos()->set_x(pos.x.raw());
  srl.mutable_pos()->set_y(pos.y.raw());
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  this->send_message("MOVE", srl);
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

const Selection& GameClient::get_selection() const
{
  return this->current_selection;
}

void GameClient::add_selection_change_callback(const t_selection_changed_callback callback)
{
  this->current_selection.on_modified_callbacks.push_back(callback);
}

void GameClient::do_new_entity(const EntityType type, const Position& pos, const uint16_t team)
{
  Entity* entity = this->world.do_new_entity(type, pos, team);
  this->camera.on_new_entity(entity);
}

void GameClient::tick()
{
  this->turn_handler.tick();
  if (this->turn_handler.is_paused())
    return;
  this->world.tick();
}
void GameClient::graphical_tick()
{
  this->camera.graphical_tick();
}

void GameClient::on_connection_closed()
{
  log_error("Connection closed by remote server");
}
