#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>
#include <cstdlib>

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

using boost::asio::ip::tcp;

class Client
{
public:
  Client(short);
  ~Client();
  void connect(boost::function< void(void) > on_success = 0, boost::function< void(void) > on_failure = 0);
  void connect_handler(boost::function< void(void) >,
		       boost::function< void(void) >,
		       const boost::system::error_code&);
  void send_handler(const boost::system::error_code&, std::size_t);
  void poll(void);
  void send(const char*);
  void send_message(void);

private:
  const short port;
  boost::asio::io_service io_service;
  tcp::socket* socket;
};

#endif /*__CLIENT_HPP__ */
