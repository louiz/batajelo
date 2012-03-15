#include <istream>

#include "server.hpp"
#include "remote_client.hpp"

Server::Server(short port):
  port(port)
{}

Server::~Server()
{
  log_debug("closing socket");
  delete this->acceptor;
}

// Callback that is called from io_service.run(), from the main (and only) thread.
// Even if there is data to read on two sockets at the same time, this handler will never be
// called twice at the same time. No need to use a mutex or lock.
void Server::install_accept_handler(void)
{
  RemoteClient* new_client = new RemoteClient(this->io_service);

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
  this->install_accept_handler();
}

void Server::run()
{
  this->accept();
  this->io_service.run();
}

// main de test
int	main(void)
{
  Config::read_conf("../config/batajelo.conf");
  Server server(7878);
  server.run();
  return 0;
}
