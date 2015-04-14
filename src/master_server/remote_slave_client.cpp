#include <master_server/remote_slave_client.hpp>
#include <master_server/master_to_slave_server.hpp>

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
  this->send_message("START", "");
}
