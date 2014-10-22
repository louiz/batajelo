#include <logging/logging.hpp>
#include <game/game_client.hpp>
#include <game/time.hpp>

GameClient::GameClient(const std::shared_ptr<Screen>& screen):
  ClientBase(),
  mod("monsters.yaml"),
  map(),
  world(&this->map, this->mod),
  occupants_handler(),
  current_selection(),
  screen(screen),
  camera(this, this->screen.get()),
  hud(this, this->screen.get())
{
  this->map.load_from_file("test6.tmx");

  this->world.set_next_turn_callback(std::bind(&GameClient::on_next_turn,
                                               this, std::placeholders::_1));

  this->connect("127.0.0.1", 7879);

  this->install_callback("OCCUPANT_LEFT",
                         std::bind(&GameClient::occupant_left_callback, this, std::placeholders::_1));
  this->install_callback("NEW_UNIT",
                         std::bind(&GameClient::new_unit_callback, this, std::placeholders::_1));
  this->install_callback("START",
                         std::bind(&GameClient::handle_start_command, this, std::placeholders::_1));
  this->install_callback("OK",
                         std::bind(&GameClient::ok_callback, this, std::placeholders::_1));
  this->install_callback("T",
                         std::bind(&GameClient::turn_callback, this, std::placeholders::_1));
  this->install_callback("PATH",
                         std::bind(&GameClient::path_callback, this, std::placeholders::_1));
  this->install_callback("BUILD",
                         std::bind(&GameClient::build_callback, this, std::placeholders::_1));
  this->install_callback("SPAWN",
                         std::bind(&GameClient::spawn_callback, this, std::placeholders::_1));

  this->screen->add_element(&this->camera, 0);
  this->screen->add_element(&this->hud, 1);
}

GameClient::~GameClient()
{
}

void GameClient::run()
{
  while (this->world.is_started() == false)
    {
      // Here be a loading screen.
      this->poll(10);
    }

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

      this->poll(0);

      // Get the elapsed time
      time2 = boost::posix_time::microsec_clock::universal_time();
      dt += time2 - time1;
      time1 = time2;

      this->screen->update(dt);
      // Update everything, based on the elapsed time
      long i = get_number_of_updates(dt);
      for (; i > 0; --i)
        {
          this->world.tick();
        }

      graph_time2 = boost::posix_time::microsec_clock::universal_time();
      graph_dt += graph_time2 - graph_time1;
      graph_time1 = graph_time2;
      i = get_number_of_graphicale_updates(graph_dt);
      for (; i > 0; --i)
        {
          this->graphical_tick();
        }
      // Draw the result on the screen. Limit to ~60 fps.
      if (fps_clock.getElapsedTime().asMicroseconds() > 10000)
        {
          this->screen->window().clear(sf::Color(70, 80, 80));
          // sf::View view(sf::FloatRect(0, 0, 960, 540));
          // window->setView(view);
          this->screen->draw();
          this->screen->window().display();
          fps_clock.restart();
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
  return this->screen->get_debug_hud();
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

GraphMap& GameClient::get_map()
{
  return this->map;
}

void GameClient::send_command(const char* name, const std::string& archive)
{
  Command* command = new Command;
  command->set_name(name);
  command->set_body(archive.data(), archive.length());
  this->send(command);
}

void GameClient::on_next_turn(unsigned long turn)
{
  if (this->started == true)
    this->confirm_turn(turn+1);
}

void GameClient::new_occupant_callback(Command* command)
{
  log_debug("new_occupant_callback");
  auto occupant = std::make_unique<Occupant>();

  std::string data(command->body, command->body_size);
  if (occupant->from_string(data) == false)
    {
      log_error("Invalid data received for the occupant.");
      return ;
    }
  log_debug("Occupant: " << occupant->name << " " << occupant->number);
  this->add_new_occupant(std::move(occupant));
}

void GameClient::occupant_left_callback(Command* command)
{
  Occupant occupant;
  if (occupant.from_string(std::string(command->body, command->body_size)) == false)
    {
      log_error("Invalid data received for the leaving occupant.");
      return ;
    }
  log_debug("Occupant to remove: " << occupant.number);
  this->occupants_handler.remove(occupant);
}

void GameClient::new_unit_callback(Command* command)
{
  DoUnitEvent* e = new DoUnitEvent(command);
  if (e->is_valid() == false)
    {
      log_debug("Invalid data for the new entity.");
      return ;
    }
  Action* action = new Action(std::bind(&World::do_new_unit, &this->world, std::placeholders::_1), e);
  log_debug("new entity");
  this->world.turn_handler.insert_action(action, e->turn);
  if (this->started == true)
    this->confirm_action(e->get_id());
  else
    action->validate_completely();
}

void GameClient::handle_start_command(Command* command)
{
  ActionEvent start_event(command);
  if (start_event.is_valid() == false)
    {
      log_warning("Invalid data for START command");
      return ;
    }
  log_debug("The first turn to start is " << start_event.turn);
  this->world.unpause();
  do
    {
      this->world.tick(true);
    } while (this->world.turn_handler.get_current_turn() < start_event.turn - 1);

  this->confirm_turn(start_event.turn);
  this->world.validate_turn_completely(start_event.turn);
  this->world.start();
}

void GameClient::ok_callback(Command* command)
{
  OkEvent ok_event(command);
  if (ok_event.is_valid() == false)
    {
      log_warning("Invalid data for OK command");
      return ;
    }
  this->world.completely_validate_action(ok_event.get_id());
}

void GameClient::turn_callback(Command* command)
{
  std::istringstream is(std::string(command->body, command->body_size));
  unsigned int number;
  is >> number;
  this->world.validate_turn_completely(number);
}

void GameClient::path_callback(Command* command)
{
  DoMoveEvent* e = new DoMoveEvent(command);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for PATH command");
      return ;
    }
  Action* action = new Action(std::bind(&World::do_path, &this->world, std::placeholders::_1), e);
  this->insert_received_action(action, e);
}

void GameClient::build_callback(Command* command)
{
  DoBuildEvent* e = new DoBuildEvent(command);
  log_info("Build_callback: " << e->actor << " " << e->x << ":" << e->y);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for BUILD command");
      return ;
    }
  Action* action = new Action(std::bind(&World::do_build, &this->world, std::placeholders::_1), e);
  this->insert_received_action(action, e);
}

void GameClient::insert_received_action(Action* action, ActionEvent* event)
{
  this->world.turn_handler.insert_action(action, event->turn);
  if (this->started == true)
    this->confirm_action(event->get_id());
  else
    action->validate_completely();
}

void GameClient::confirm_action(const unsigned long int id)
{
  Event ok_event(id);
  this->send_command("OK", ok_event.to_string());
}

void GameClient::confirm_initial_turn()
{
  this->confirm_turn(1);
}

void GameClient::confirm_turn(const unsigned int number)
{
  std::ostringstream os;
  os << number;
  this->send_command("T", os.str());
}

bool GameClient::action_build(const unsigned int x, const unsigned y, const std::size_t id)
{
  log_info("action_build: " << x << ":" << y << "=" << id);
  assert(this->current_selection.is_empty() == false);
  BuildEvent event;
  event.actor = this->current_selection.get_entities().front()->id;
  Position pos(x, y);
  std::tie(event.x, event.y) = this->world.get_cell_at_position(pos);
  event.type_id = id;
  this->send_command("BUILD", event.to_string());
  return true;
}

void GameClient::action_spawn(const t_left_click left_click)
{
  assert(this->current_selection.is_empty() == false);
  SpawnEvent event;
  event.actor = this->current_selection.get_entities().front()->id;
  event.type_id = left_click.id;
  this->send_command("SPAWN", event.to_string());
}

bool GameClient::action_move(const unsigned int x, const unsigned y, const std::size_t)
{
  MoveEvent event;
  for (const auto entity: this->world.entities)
    {
      if (this->is_entity_selected(entity))
        event.actors_ids.push_back(entity->get_id());
    }
  if (event.actors_ids.size() == 0)
    return false;
  event.x = x;
  event.y = y;
  this->send_command("MOVE", event.to_string());
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

void GameClient::spawn_callback(Command* command)
{
  DoSpawnEvent* e = new DoSpawnEvent(command);
  log_info("Spawn_callback: " << e->actor << " " << e->type_id);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for Spawn command");
      return ;
    }
  Action* action = new Action(std::bind(&World::do_spawn, &this->world, std::placeholders::_1), e);
  this->insert_received_action(action, e);
}

void GameClient::insert_unit(Unit* unit)
{
  this->camera.on_new_unit(unit);
  this->world.insert_unit(unit);
}

void GameClient::insert_building(Building* building)
{
  this->camera.on_new_building(building);
  this->world.insert_building(building);
}

void GameClient::graphical_tick()
{
  this->camera.graphical_tick();
}

void GameClient::on_connection_closed()
{
  log_error("Connection closed by remote server");
}
