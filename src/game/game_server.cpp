#include <game/game_server.hpp>
#include <algorithm>

GameServer::GameServer(short port, Mod& mod):
  Server<RemoteGameClient>(port),
  world(nullptr, mod),
  occupants()
{
}

GameServer::~GameServer()
{
}

World* GameServer::get_world()
{
  return &this->world;
}

void GameServer::on_new_client(RemoteGameClient* new_client)
{
  // Give a pointer to ourself (the server) to this new client, so that it
  // can communicate with us later
  new_client->set_game_server(this);

  // First send the new player to all connected clients.
  // (including the just-new-connected one)
  Command* command = new Command();
  command->set_name("NEW_OCCUPANT");

  // Occupant* occupant = new Occupant(new_client->get_number(), "coucou");
  // this->world->add_new_occupant(occupant);
  auto new_occupant = std::make_unique<Occupant>(new_client->get_number(), "coucou");

  command->set_body(new_occupant->to_string().c_str());
  this->send_to_all_clients(command);

  this->occupants.add(std::move(new_occupant));
  log_debug("There are now " << this->occupants.size() << " occupants");

  // Then send the list of already-connected clients to the new
  // one.
  for (const auto& occupant: this->occupants)
    {
      if (occupant->number != new_client->get_number())
     { // Do not send the information about itself, it
       // already just received that.
       command = new Command();
       command->set_name("NEW_OCCUPANT");
       command->set_body(occupant->to_string().c_str());
       new_client->send(command);
     }
    }
  // Send the replay to the new client.
  this->send_replay(new_client);
  // Send an action to indicate at what turn the replay ends
  // and where the game currently is.
  this->send_start_command(new_client);
  // Send the futur commands to the new client
  // and change their validations_needed value, since
  // they now require one more validation.
  this->send_and_adjust_future_commands(new_client);
  // TODO start it somewhere else.
  this->start_game();
}

void GameServer::send_replay(RemoteGameClient* new_client)
{
  Action* action;
  Command* command;
  ActionEvent* event;
  for (const auto& action: this->world.get_replay().get_actions())
    {
      event = action->get_event();
      command = new Command;
      command->set_name(event->name);
      command->set_body(event->to_string().c_str());
      new_client->send(command);
    }
}

void GameServer::send_and_adjust_future_commands(RemoteGameClient* new_client)
{
  Turn* turn;
  Action* action;
  Command* command;
  ActionEvent* event;
  std::size_t occupants = this->occupants.size();

  for (auto& turn: this->world.turn_handler.get_turns())
    {
      for (auto& action: turn.get_actions())
        {
          if (action->is_completely_validated() == false)
            {
              action->set_validations_needed(occupants);
              event = action->get_event();
              command = new Command;
              command->set_name(event->name);
              command->set_body(event->to_string().c_str());
              new_client->send(command);
            }
        }
    }
}

void GameServer::adjust_and_revalidate_futur_commands()
{
  Turn* turn;
  Action* action;
  std::size_t occupants = this->occupants.size();
  unsigned long turn_id = this->world.turn_handler.get_current_turn();

  for (auto& turn: this->world.turn_handler.get_turns())
    {
      if (turn.get_number_of_validations() == occupants)
        {
          this->send_turn(turn_id, 0);
          turn.validate_completely();
        }
      for (auto& action: turn.get_actions())
        {
          if (action->is_completely_validated() == false)
            {
              action->set_validations_needed(occupants);
              if (action->is_validated() == true)
                { // If the action was validated by all clients
                  // except the one that just left, the action is now
                  // completely validated.
                  action->validate_completely();
                  this->send_ok(action->get_event()->get_id(), 0);
                }
            }
        }
      ++turn_id;
    }
}


void GameServer::send_start_command(RemoteGameClient* client)
{
  Command* command = new Command;
  ActionEvent start_event("START");
  start_event.turn = this->world.turn_handler.get_current_turn() + 1;
  command->set_name("START");
  command->set_body(start_event.to_string().c_str());
  client->send(command);
}

void GameServer::on_client_left(RemoteGameClient* client)
{
  const auto it = std::remove_if(this->occupants.begin(), this->occupants.end(),
                           [client](const auto& occupant)
                                 {
                                   return occupant->number == client->get_number();
                                 });
  std::for_each(it, this->occupants.end(),
               [this](const auto& occupant)
               {
                 Command* command = new Command;
                 command->set_name("OCCUPANT_LEFT");
                 command->set_body(occupant->to_string().c_str());
                 this->send_to_all_clients(command);
               });
  this->occupants.erase(it, this->occupants.end());
}

void GameServer::tick()
{
  this->world.tick();
}

void GameServer::pause_game()
{
  this->world.pause();
}

void GameServer::unpause_game()
{
  this->world.unpause();
}

void GameServer::start_game()
{
  this->world.start();
}

void GameServer::send_command_to_all(const char* name, const std::string& data)
{
  Command* command = new Command;
  command->set_name(name);
  command->set_body(data.data(), data.size());
  this->send_to_all_clients(command);
}

void GameServer::send_ok(const unsigned int id, const unsigned long int by)
{
  // TODO the 'by' value is useless here.
  // Even the OkEvent is useless, we just need to pass an unsigned int.
  Command* command = new Command;
  command->set_name("OK");
  OkEvent ok_event(id, by);
  command->set_body(ok_event.to_string().c_str());
  this->send_to_all_clients(command);
}

void GameServer::send_turn(const unsigned int id, const unsigned long int by)
{
  // TODO Same here, remove the by.
  Command* command = new Command;
  command->set_name("T");
  std::ostringstream os;
  os << id;
  command->set_body(os.str().c_str());
  this->send_to_all_clients(command);
}

void GameServer::spawn_unit(const size_t type, const int x, const int y)
{
  Unit* new_unit = this->world.create_unit(type);
  new_unit->pos = Position(x, y);
  DoUnitEvent* e = new DoUnitEvent(new_unit);
  e->turn = 1;
  Action* action = new Action(nullptr, e, 0);
  action->validate_completely();
  this->world.replay.insert_action(action);
}

void GameServer::init()
{
  log_error("Creating initial world state");
  this->spawn_unit(0, 300, 300);
  // this->spawn_unit(0, 0, 400);
  // this->spawn_unit(0, 0, 600);

  // this->spawn_unit(0, 200, 0);
  // this->spawn_unit(0, 400, 0);
  // this->spawn_unit(0, 600, 0);

  // this->spawn_unit(0, 200, 200);
  // this->spawn_unit(0, 400, 400);
  // this->spawn_unit(0, 600, 600);
  log_debug("done");
}

void GameServer::move_callback(Command* command)
{
  MoveEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for MOVE command");
      return ;
    }
  DoMoveEvent* path_event = new DoMoveEvent(event);
  unsigned long current_turn = this->world.turn_handler.get_current_turn();
  log_debug("Currently at: " << current_turn);
  log_debug("move_callback: " << path_event->to_string());
  path_event->turn = current_turn + 2;
  this->send_command_to_all("PATH", path_event->to_string());

  Action* action = new Action(nullptr, path_event, this->occupants.size());
  this->world.turn_handler.insert_action(action, path_event->turn);
}

void GameServer::build_callback(Command* command)
{
  BuildEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for BUILD command");
      return ;
    }
  // Here check for command validity etc etc.
  // if the command is valid, just return a DoBuildEvent with the same value
  DoBuildEvent* doevent = new DoBuildEvent(event);
  doevent->turn = this->world.turn_handler.get_current_turn() + 2;
  this->send_command_to_all("BUILD", doevent->to_string());
  Action* action = new Action(nullptr, doevent, this->occupants.size());
  this->world.turn_handler.insert_action(action, doevent->turn);
}

void GameServer::spawn_callback(Command* command)
{
  SpawnEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for SPAWN command");
      return ;
    }
  DoSpawnEvent* doevent = new DoSpawnEvent(event);
  doevent->turn = this->world.turn_handler.get_current_turn() + 2;
  this->send_command_to_all("SPAWN", doevent->to_string());
  Action* action = new Action(nullptr, doevent, this->occupants.size());
  this->world.turn_handler.insert_action(action, doevent->turn);
}

bool GameServer::validate_action(const unsigned int id, const unsigned long int by)
{
  log_debug("Action " << id << " validated by " << by);
  bool res = this->world.turn_handler.validate_action(id, by);
  log_debug("This made it completely validated:" << res);
  return res;
}

bool GameServer::validate_turn(const unsigned int id, const unsigned long int by)
{
  return this->world.turn_handler.validate_turn(id, by, this->occupants.size());
}
