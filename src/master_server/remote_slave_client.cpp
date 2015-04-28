#include <master_server/remote_slave_client.hpp>
#include <master_server/master_to_slave_server.hpp>
#include <database/database.hpp>

#include "slave.pb.h"

void RemoteSlaveClient::set_server(MasterToSlaveServer* server)
{
  this->server = server;
}

void RemoteSlaveClient::on_connection_closed()
{
  this->server->remove_client(this);
}

void RemoteSlaveClient::install_callbacks()
{
  ser::slave::StartGameRequest req;

  auto game = this->server->get_database()->create_new_game();

  log_debug("Game id created: " << game.id);
  req.set_game_id(game.id);
  this->send_message("START", req);
}
