#ifndef MASTERTOCLIENT_SERVER_HPP_INCLUDED
#define MASTERTOCLIENT_SERVER_HPP_INCLUDED

#include <network/server.hpp>
#include <master_server/remote_client.hpp>

class MasterServer;
class Database;

class MasterToClientServer: public Server<RemoteClient>
{
public:
  MasterToClientServer(MasterServer* master,
                       const short port);
  ~MasterToClientServer() = default;
  /**
   * Search for a connected client with this login
   * @return RemoteClient* can be nullptr if nothing is found
   */
  const RemoteClient* find_client_by_login(const std::string& login) const;
  void on_new_client(RemoteClient* client) override final;
  void on_client_left(RemoteClient* client) override final;
  Database* get_database();

private:
  MasterServer* master;

  MasterToClientServer(const MasterToClientServer&) = delete;
  MasterToClientServer(MasterToClientServer&&) = delete;
  MasterToClientServer& operator=(const MasterToClientServer&) = delete;
  MasterToClientServer& operator=(MasterToClientServer&&) = delete;
};

#endif /* MASTERTOCLIENT_SERVER_HPP_INCLUDED */
