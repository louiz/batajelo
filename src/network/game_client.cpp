#include <network/game_client.hpp>

GameClient::GameClient()
{
}

GameClient::~GameClient()
{
}

void GameClient::install_callbacks()
{
}

void GameClient::on_connection_closed()
{
  log_debug("connection closed");
}
