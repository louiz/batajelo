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

void GameClient::send_action(const std::string& data)
{
  Command* command = new Command;
  command->set_name("ACTION");
  command->set_body(data.data(), data.length());
  this->send(command);
}
