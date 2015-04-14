#ifndef MASTERTOSLAVE_SERVER_HPP_INCLUDED
#define MASTERTOSLAVE_SERVER_HPP_INCLUDED

#include <network/server.hpp>
#include <master_server/remote_slave_client.hpp>

class MasterServer;

class MasterToSlaveServer: public Server<RemoteSlaveClient>
{
public:
  MasterToSlaveServer(MasterServer* master,
                      const short port);
  ~MasterToSlaveServer() = default;

  void on_new_client(RemoteSlaveClient* client) override final;
  void on_client_left(RemoteSlaveClient* client) override final;

private:
  MasterServer* master;

  MasterToSlaveServer(const MasterToSlaveServer&) = delete;
  MasterToSlaveServer(MasterToSlaveServer&&) = delete;
  MasterToSlaveServer& operator=(const MasterToSlaveServer&) = delete;
  MasterToSlaveServer& operator=(MasterToSlaveServer&&) = delete;
};

#endif /* MASTERTOSLAVE_SERVER_HPP_INCLUDED */
