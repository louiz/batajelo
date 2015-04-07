#include <game/remote_game_client.hpp>
#include <game/game_server.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <world/world.hpp>

RemoteGameClient::RemoteGameClient():
  server(nullptr)
{
}

RemoteGameClient::~RemoteGameClient()
{
}

void RemoteGameClient::set_game_server(GameServer* server)
{
  this->server = server;
}

void RemoteGameClient::on_connection_closed()
{
  this->server->remove_client(this);
}

void RemoteGameClient::install_callbacks()
{
  this->install_callback("MOVE", std::bind(&GameServer::on_move_request, this->server, std::placeholders::_1));
  this->install_callback("CAST", std::bind(&GameServer::on_cast_request, this->server, std::placeholders::_1));
}
