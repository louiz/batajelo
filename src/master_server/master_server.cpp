#include <master_server/master_server.hpp>

MasterServer::MasterServer(const short port):
  Server<RemoteClient>(port)
{
}

const RemoteClient* MasterServer::find_client_by_login(const std::string& login) const
{
  for (const RemoteClient* client: this->clients)
    {
      const auto user = client->get_user();
      if (user && user->login == login)
        return client;
    }
  return nullptr;
}

Database* MasterServer::get_database()
{
  return &this->database;
}

void MasterServer::on_new_client(RemoteClient* client)
{
  client->set_server(this);
}

void MasterServer::on_client_left(RemoteClient* client)
{
  log_debug("Client left");
}
