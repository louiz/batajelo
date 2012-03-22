// BaseServer is the server handling clients authentication,
// serving menus, history, players’ informations, etc

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <network/remote_client.hpp>

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

using boost::asio::ip::tcp;

class Server
{
public:
  Server(short port);
  ~Server();
  void run(void);

private:
  void install_accept_handler(void);
  void handle_accept(RemoteClient*, const boost::system::error_code&);
  void accept(void);

  boost::asio::io_service io_service;
  tcp::acceptor* acceptor;
  short port;
};

#endif /*__SERVER_HPP__ */
