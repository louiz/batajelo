#ifndef MASTER_SERVER_HPP_INCLUDED
#define MASTER_SERVER_HPP_INCLUDED

#include <network/server.hpp>
#include <master_server/remote_client.hpp>
#include <database/database.hpp>

class MasterServer: public Server<RemoteClient>
{
public:
  MasterServer(const short port);
  ~MasterServer() = default;
  /**
   * Search for a connected client with this login
   * @return RemoteClient* can be nullptr if nothing is found
   */
  const RemoteClient* find_client_by_login(const std::string& login) const;
  void on_new_client(RemoteClient* client) override final;
  void on_client_left(RemoteClient* client) override final;
  Database* get_database();

private:
  Database database;
  MasterServer(const MasterServer&) = delete;
  MasterServer(MasterServer&&) = delete;
  MasterServer& operator=(const MasterServer&) = delete;
  MasterServer& operator=(MasterServer&&) = delete;
};

#endif /* MASTER_SERVER_HPP_INCLUDED */
