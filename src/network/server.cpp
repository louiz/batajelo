#include <istream>

#include <network/server.hpp>
#include <network/remote_client.hpp>

Server::Server(short port):
  port(port)
{}

Server::~Server()
{
  log_debug("closing socket");
  delete this->acceptor;
}

void Server::install_accept_handler(void)
{
  RemoteClient* new_client = new RemoteClient(this->io_service, this);

  this->acceptor->async_accept(new_client->get_socket(),
			       boost::bind(&Server::handle_accept, this, new_client,
					   boost::asio::placeholders::error));
}

void Server::accept(void)
{
  boost::system::error_code error;

  this->acceptor = new tcp::acceptor(this->io_service, tcp::endpoint(tcp::v4(), this->port));
  error = this->acceptor->listen(512, error);
  if (error)
    {
      log_error("Error on listen() [" << error << "]. Exiting.");
      return;
    }
  this->install_accept_handler();
}

void Server::handle_accept(RemoteClient* client, const boost::system::error_code& error)
{
  client->start();
  this->clients.push_back(client);
  this->install_accept_handler();
}

void Server::run()
{
  this->accept();
  this->io_service.run();
}

void Server::remove_client(RemoteClient* client)
{
  std::vector<RemoteClient*>::iterator it;
  for (it = this->clients.begin(); it < this->clients.end(); ++it)
    if (*it == client)
      this->clients.erase(it);
  delete client;
}

RemoteClient* Server::find_client_by_login(const std::string& login)
{
  std::vector<RemoteClient*>::iterator it;
  for (it = this->clients.begin(); it < this->clients.end(); ++it)
    if ((*it)->get_user() && (*it)->get_user()->get("login") == login)
      return *it;
  return 0;
}

int main(int argc, char *argv[])
{
  if (!Config::read_conf("../../batajelo.conf"))
    return 1;
  Server s(7878);
  s.run();
  return 0;
}
