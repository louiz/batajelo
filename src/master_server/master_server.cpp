#include <master_server/master_server.hpp>

MasterServer::MasterServer(const short client_port,
                           const short slave_port):
  to_client_server(this, client_port),
  to_slave_server(this, slave_port)
{
}

Database* MasterServer::get_database()
{
  return &this->database;
}

void MasterServer::run()
{
  this->to_client_server.start();
  this->to_slave_server.start();
  while (true)
    IoService::get().poll();
}
