#include <network/remote_game_client.hpp>
#include <network/game_server.hpp>
#include <boost/archive/text_iarchive.hpp>

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
  this->install_callback("ACTION", boost::bind(&RemoteGameClient::action_callback, this, _1));
}

void RemoteGameClient::action_callback(Command* command)
{
  World* world = static_cast<GameServer*>(this->server)->get_world();
  Action action;

  std::istringstream iss(std::string(command->body, command->body_size));
  boost::archive::text_iarchive archive(iss, boost::archive::no_header);
  archive >> action;
  log_debug("The action is: " << action.x << ":" << action.y << ". ");
}

boost::asio::io_service& RemoteGameClient::get_io_service()
{
  return this->server->io_service;
}
