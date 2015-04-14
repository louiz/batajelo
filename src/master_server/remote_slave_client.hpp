#ifndef REMOTE_SLAVE_CLIENT_HPP_INCLUDED
#define REMOTE_SLAVE_CLIENT_HPP_INCLUDED

#include <network/remote_client_base.hpp>

class MasterToSlaveServer;

class RemoteSlaveClient: public RemoteClientBase
{
public:
  RemoteSlaveClient() = default;
  ~RemoteSlaveClient() = default;
  void on_connection_closed() override final;
  void set_server(MasterToSlaveServer* server);

private:
  MasterToSlaveServer* server;
  void install_callbacks() override final;

  RemoteSlaveClient(const RemoteSlaveClient&) = delete;
  RemoteSlaveClient(RemoteSlaveClient&&) = delete;
  RemoteSlaveClient& operator=(const RemoteSlaveClient&) = delete;
  RemoteSlaveClient& operator=(RemoteSlaveClient&&) = delete;
};

#endif /* REMOTE_SLAVE_CLIENT_HPP_INCLUDED */
