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

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <network/remote_client.hpp>

using boost::asio::ip::tcp;

class Server
{
public:
  /**
   * Create the server instance. Use run() to start its loop.
   * @param port The port on which the servers accepts new connections.
   */
  Server(short port);
  ~Server();
  /**
   * Starts the main loop.
   * @return void
   */
  void run(void);
  /**
   * To be called by the a RemoteClient instance, to delete itself from
   * the RemoteClient list.
   */
  void remove_client(RemoteClient*);
  /**
   * Search for a connected client with this login
   * @return RemoteClient* can be NULL if nothing is found
   */
  RemoteClient* find_client_by_login(const std::string&);

private:
  void install_accept_handler(void);
  void handle_accept(RemoteClient*, const boost::system::error_code&);
  void accept(void);

  std::vector<RemoteClient*> clients;
  boost::asio::io_service io_service;
  tcp::acceptor* acceptor;
  short port;
};

#endif /*__SERVER_HPP__ */
/**#@}*/
