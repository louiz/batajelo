#ifndef MASTER_SERVER_HPP_INCLUDED
#define MASTER_SERVER_HPP_INCLUDED

#include <master_server/master_to_client_server.hpp>
#include <master_server/master_to_slave_server.hpp>

#include <database/database.hpp>

class MasterServer
{
public:
  MasterServer(const short client_port, const short slave_port);
  ~MasterServer() = default;
  Database* get_database();
  void run();

private:
  MasterToClientServer to_client_server;
  MasterToSlaveServer to_slave_server;

  Database database;

  MasterServer(const MasterServer&) = delete;
  MasterServer(MasterServer&&) = delete;
  MasterServer& operator=(const MasterServer&) = delete;
  MasterServer& operator=(MasterServer&&) = delete;
};

#endif /* MASTER_SERVER_HPP_INCLUDED */
