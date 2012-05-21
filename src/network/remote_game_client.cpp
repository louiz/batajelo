#include <network/remote_game_client.hpp>
#include <network/game_server.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <world/world.hpp>

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
  World* world = static_cast<GameServer*>(this->server)->get_world();

  this->install_callback("OK", boost::bind(&RemoteGameClient::ok_callback, this, _1));
  this->install_callback("MOVE", boost::bind(&World::move_callback, world, _1));
  // this->install_callback("MOVE", boost::bind(&World::try_move, world, _1));
}

boost::asio::io_service& RemoteGameClient::get_io_service()
{
  return this->server->io_service;
}

void RemoteGameClient::ok_callback(Command* command)
{
  Event ok_event(command);
  World* world = static_cast<GameServer*>(this->server)->get_world();

  this->send_ok(ok_event.get_id(), this->get_number());
  world->validate_action(ok_event.get_id(), this->get_number());
}

void RemoteGameClient::send_ok(const unsigned int id, const unsigned long int by)
{
  Command* command = new Command;
  command->set_name("OK");
  OkEvent ok_event(id, by);
  command->set_body(ok_event.to_string().c_str());
  this->server->send_to_all_clients(command);
}
