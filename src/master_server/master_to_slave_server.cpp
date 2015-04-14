#include <master_server/master_to_slave_server.hpp>

#include <logging/logging.hpp>

MasterToSlaveServer::MasterToSlaveServer(MasterServer* master,
                                         const short port):
  Server<RemoteSlaveClient>(port),
  master(master)
{
}

void MasterToSlaveServer::on_new_client(RemoteSlaveClient* client)
{
  log_debug("Starting RemoteSlaveClient");
  client->set_server(this);
}

void MasterToSlaveServer::on_client_left(RemoteSlaveClient* client)
{
  log_debug("Ending RemoteSlaveClient");
}
