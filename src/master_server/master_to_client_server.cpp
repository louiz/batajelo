#include <master_server/master_to_client_server.hpp>
#include <master_server/master_server.hpp>

MasterToClientServer::MasterToClientServer(MasterServer* master,
                                           const short port):
  Server<RemoteClient>(port),
  master(master)
{
  RemoteClient::context.set_options(  boost::asio::ssl::context::default_workarounds
                                    | boost::asio::ssl::context::no_sslv2
                                    | boost::asio::ssl::context::no_sslv3);

  RemoteClient::context.set_verify_mode(boost::asio::ssl::verify_none);
  RemoteClient::context.use_certificate_file("../bata_test.cert",
                                             boost::asio::ssl::context::pem);
  RemoteClient::context.use_private_key_file("../bata_test.key",
                                             boost::asio::ssl::context::pem);
}

const RemoteClient* MasterToClientServer::find_client_by_login(const std::string& login) const
{
  for (const auto& client: this->clients)
    {
      const auto user = client->get_user();
      if (user && user->login == login)
        return client.get();
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
