#include <network/remote_game_client.hpp>

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
  this->install_callback("MOVE", boost::bind(&RemoteGameClient::move_callback, this, _1));
}

void RemoteGameClient::move_callback(Command* command)
{
  log_debug("move command");
}

boost::asio::io_service& RemoteGameClient::get_io_service()
{
  return this->server->io_service;
}
