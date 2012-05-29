#include <network/game_server.hpp>

GameServer::GameServer(short port):
  Server<RemoteGameClient>(port)
{
  this->world = new ServerWorld;
}

GameServer::~GameServer()
{
}

ServerWorld* GameServer::get_world() const
{
  return this->world;
}

void GameServer::on_new_client(RemoteGameClient* new_client)
{
  // First send the new player to all connected clients.
  // (including the just-new-connected one)
  Command* command = new Command();
  command->set_name("NEW_OCCUPANT");

  Occupant* occupant = new Occupant(new_client->get_number(), "coucou");
  this->world->add_new_occupant(occupant);
  log_debug("There are now " << this->world->occupants.size() << " occupants");

  command->set_body(occupant->to_string().c_str());
  this->send_to_all_clients(command);

  // Then send the list of already-connected clients to the new
  // one.
  std::vector<Occupant*>::iterator it;
  for (it = this->world->occupants.begin(); it < this->world->occupants.end(); ++it)
    {
      if ((*it)->number != new_client->get_number())
	{ // Do not send the information about itself, it
	  // already just received that.
	  command = new Command();
	  command->set_name("NEW_OCCUPANT");
	  command->set_body((*it)->to_string().c_str());
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
  Replay* replay = this->world->get_replay();
  replay->reset_action_iterator();
  Action* action;
  Command* command;
  ActionEvent* event;
  while ((action = replay->get_next_action()) != 0)
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
  TurnHandler* turn_handler = this->world->get_turn_handler();
  unsigned int occupants = this->world->get_number_of_occupants();
  turn_handler->reset_turns_iterator();
  while ((turn = turn_handler->get_next_turn()) != 0)
    {
      turn->reset_action_iterator();
      while ((action = turn->get_next_action()) != 0)
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


void GameServer::send_start_command(RemoteGameClient* client)
{
  Command* command = new Command;
  ActionEvent start_event("START");
  TurnHandler* turn_handler = this->world->get_turn_handler();
  start_event.turn = turn_handler->get_current_turn() + 1;
  command->set_name("START");
  command->set_body(start_event.to_string().c_str());
  client->send(command);
}

void GameServer::on_client_left(RemoteGameClient* client)
{
  Command* command = new Command;
  command->set_name("OCCUPANT_LEFT");
  std::vector<Occupant*>::iterator it;
  Occupant* occupant_to_remove;

  for (it = this->world->occupants.begin(); it < this->world->occupants.end(); ++it)
    {
      if ((*it)->number == client->get_number())
	{
	  occupant_to_remove = (*it);
	  command->set_body(occupant_to_remove->to_string().c_str());
	  this->send_to_all_clients(command);
	  this->world->occupants.erase(it);
	  delete occupant_to_remove;
	  return ;
	}
    }
  assert(false);
}

void GameServer::tick()
{
  this->world->tick();
}

void GameServer::pause_game()
{
  this->world->pause();
}

void GameServer::unpause_game()
{
  this->world->unpause();
}

void GameServer::start_game()
{
  this->world->start();
}

void GameServer::send_pending_commands()
{
  Command* command;
  while ((command = this->world->get_pending_command()))
    {
      log_debug("PENDING COMMAND!");
      this->send_to_all_clients(command);
    }
}
