#include <master_server/master_to_client_server.hpp>
#include <master_server/master_server.hpp>

MasterToClientServer::MasterToClientServer(MasterServer* master,
                                           const short port):
  Server<RemoteClient>(port),
  master(master)
{
}

const RemoteClient* MasterToClientServer::find_client_by_login(const std::string& login) const
{
  for (const RemoteClient* client: this->clients)
    {
      const auto user = client->get_user();
      if (user && user->login == login)
        return client;
    }
  return nullptr;
}

void MasterToClientServer::on_new_client(RemoteClient* client)
{
  log_debug("starting RemoteClient");
  client->set_server(this);
}

void MasterToClientServer::on_client_left(RemoteClient* client)
{
  log_debug("RemoteClient left");
}

Database* MasterToClientServer::get_database()
{
  return this->master->get_database();
}
