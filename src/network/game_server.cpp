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
