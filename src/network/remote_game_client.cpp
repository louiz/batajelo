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

  this->install_callback("MOVE", boost::bind(&World::try_move, world, _1));
}

boost::asio::io_service& RemoteGameClient::get_io_service()
{
  return this->server->io_service;
}
