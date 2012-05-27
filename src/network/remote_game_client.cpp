#include <network/remote_game_client.hpp>
#include <network/game_server.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <world/server_world.hpp>

RemoteGameClient::RemoteGameClient(boost::asio::io_service& io_service,
			   Server<RemoteGameClient>* server):
  InterfaceRemoteClient(io_service),
  server(server)
{
}

RemoteGameClient::~RemoteGameClient()
{
}

void RemoteGameClient::on_connection_closed()
{
  this->server->remove_client(this);
}

void RemoteGameClient::install_callbacks()
{
  ServerWorld* world = static_cast<GameServer*>(this->server)->get_world();

  this->install_callback("OK", boost::bind(&RemoteGameClient::ok_callback, this, _1));
  this->install_callback("T", boost::bind(&RemoteGameClient::turn_callback, this, _1));
  this->install_callback("MOVE", boost::bind(&ServerWorld::move_callback, world, _1));
}

boost::asio::io_service& RemoteGameClient::get_io_service()
{
  return this->server->io_service;
}

void RemoteGameClient::ok_callback(Command* command)
{
  // todo replace that Event struct by just an int.
  Event ok_event(command);
  if (ok_event.is_valid() == false)
    {
      log_warning("Invalid data for OK command");
      return ;
    }
  ServerWorld* world = static_cast<GameServer*>(this->server)->get_world();

  if (world->validate_action(ok_event.get_id(), this->get_number()) == true)
    this->send_ok(ok_event.get_id(), this->get_number());
}

void RemoteGameClient::turn_callback(Command* command)
{
  std::istringstream is(std::string(command->body, command->body_size));
  unsigned int number;
  is >> number;
  ServerWorld* world = static_cast<GameServer*>(this->server)->get_world();
  if (world->validate_turn(number, this->get_number()) == true)
    this->send_turn(number, this->get_number());
}

void RemoteGameClient::send_ok(const unsigned int id, const unsigned long int by)
{
  Command* command = new Command;
  command->set_name("OK");
  OkEvent ok_event(id, by);
  command->set_body(ok_event.to_string().c_str());
  this->server->send_to_all_clients(command);
}

void RemoteGameClient::send_turn(const unsigned int id, const unsigned long int by)
{
  Command* command = new Command;
  command->set_name("T");
  std::ostringstream os;
  os << id;
  command->set_body(os.str().c_str());
  this->server->send_to_all_clients(command);
}

