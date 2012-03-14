#include "command_handler.hpp"

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>
#include <cstdlib>

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

using boost::asio::ip::tcp;

class Client: public CommandHandler
{
public:
  Client();
  ~Client();
  void connect(const std::string&,
	       const short&,
	       boost::function< void(void) > on_success = 0,
	       boost::function< void(void) > on_failure = 0);
  void connect_handler(boost::function< void(void) >,
		       boost::function< void(void) >,
		       const boost::system::error_code&);
  void poll(void);

private:
  boost::asio::io_service io_service;
};

#endif /*__CLIENT_HPP__ */
