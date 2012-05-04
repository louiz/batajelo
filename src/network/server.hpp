/** @addtogroup Network
 *  @{
 */

/**
 * The server handling clients authentication, serving menus, history, players’
 * informations, etc
 * @class Server
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <istream>

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

using boost::asio::ip::tcp;

template <class T>
class Server
{
public:
  /**
   * Create the server instance. Use run() to start its loop.
   * @param port The port on which the servers accepts new connections.
   */
  Server(short port):
    port(port)
  {
  }
  ~Server()
  {
    log_debug("closing socket");
    delete this->acceptor;
  }
  /**
   * Starts the main loop.
   * @return void
   */
  void run(void)
  {
    this->accept();
    this->io_service.run();
  }
  /**
   * To be called by the a RemoteClient instance, to delete itself from
   * the RemoteClient list.
   */
  void remove_client(T* client)
  {
    typename std::vector<T*>::iterator it;
    for (it = this->clients.begin(); it < this->clients.end(); ++it)
      if (*it == client)
	this->clients.erase(it);
    delete client;
  }
  /**
   * Search for a connected client with this login
   * @return RemoteClient* can be NULL if nothing is found
   */
  T* find_client_by_login(const std::string& login)
  {
    typename std::vector<T*>::iterator it;
    for (it = this->clients.begin(); it < this->clients.end(); ++it)
      if ((*it)->get_user() && (*it)->get_user()->get("login") == login)
	return *it;
    return 0;
  }

  boost::asio::io_service io_service;

private:
  void install_accept_handler(void)
  {
    T* new_client = new T(this->io_service, this);

    this->acceptor->async_accept(new_client->get_socket(),
				 boost::bind(&Server<T>::handle_accept, this, new_client,
					     boost::asio::placeholders::error));
  }

  void handle_accept(T* client, const boost::system::error_code& error)
  {
    if (error)
      {
	log_error("handle_accept failed: "<< error);
	exit(1);
      }
  client->start();
  this->clients.push_back(client);
  this->install_accept_handler();
  }

  void accept(void)
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

  std::vector<T*> clients;
  tcp::acceptor* acceptor;
  short port;
};

#endif /*__SERVER_HPP__ */
/**#@}*/
