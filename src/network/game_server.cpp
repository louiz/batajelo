#include <network/game_server.hpp>

GameServer::GameServer(short port):
  Server<RemoteGameClient>(port)
{
  this->world = new World;
}

GameServer::~GameServer()
{
}

World* GameServer::get_world() const
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

  // Spawn a new unit for each new player.
  // Only for tests, TODO: remove that.
  Entity* new_entity = this->world->create_entity(0);
  command = new Command();
  new_entity->y = rand() % 200;
  new_entity->x = rand() % 200;

  command->set_name("NEW_ENTITY");
  command->set_body(new_entity->to_string().c_str());
  log_debug(new_entity->to_string());
  this->send_to_all_clients(command);
  Entity* entity;
  while ((entity = this->world->get_next_entity()))
    {
      command = new Command();
      command->set_name("NEW_ENTITY");
      command->set_body(entity->to_string().c_str());
      new_client->send(command);
    }
  this->world->insert_entity(new_entity);

  // Finally send a START command to the new player, used to synchronize the
  // start of the game with others.
  Event* start_event = new Event;
  Command* start_command = new Command;
  start_command->set_name("START");
  start_command->set_body(start_event->to_string().c_str());
  this->send_to_all_clients(start_command);

  this->world->install_start_action(start_event, 1);

  // TODO remove that.
  this->start_game();
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
