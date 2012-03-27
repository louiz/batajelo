/** @addtogroup Network
 *  @{
 */

/**
 * The main network class for the client.
 * Connects to the remote server, sends requests to it and
 * calls the appropriate callbacks when the answer is received (or
 * when a standalone command is received from the server)
 * @class Client
 */

#include <network/command_handler.hpp>

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
  /**
   * Tries to connect to the remote server. Calls one of the given callbacks, if it
   * succeeds or fails.
   * @return void
   */
  void connect(const std::string&,
	       const short&,
	       boost::function< void(void) > on_success = 0,
	       boost::function< void(void) > on_failure = 0);
  /**
   * Check for data on the sockets (to send or write), and execute the
   * appropriate handlers. Does nothing if there’s nothing to read or write.
   * This is non-blocking.
   */
  void poll(void);

  virtual void on_connection_closed() {}

private:
  void connect_handler(boost::function< void(void) >,
		       boost::function< void(void) >,
		       const boost::system::error_code&);

  boost::asio::io_service io_service;
};

#endif /*__CLIENT_HPP__ */
/**@}*/
