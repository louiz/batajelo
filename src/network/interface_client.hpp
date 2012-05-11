/** @addtogroup Network
 *  @{
 */

/**
 * The main network class for the clients.
 * Connects to the remote server, sends requests to it and
 * calls the appropriate callbacks when the answer is received (or
 * when a standalone command is received from the server)
 * @class InterfaceClient
 */

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>
#include <cstdlib>

#ifndef __INTERFACE_CLIENT_HPP__
# define __INTERFACE_CLIENT_HPP__

#include <network/command_handler.hpp>
#include <network/ping_handler.hpp>
#include <network/timed_event_handler.hpp>
#include <network/command.hpp>
#include <network/timed_event.hpp>

using boost::asio::ip::tcp;

class InterfaceClient: public CommandHandler, public TimedEventHandler, public PingHandler
{
public:
  InterfaceClient();
  ~InterfaceClient();
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
   * Install all the default callbacks.
   */
  virtual void install_callbacks() = 0;
  /**
   * Check for data on the sockets (to send or write), and execute the
   * appropriate handlers. Does nothing if there’s nothing to read or write.
   * This is non-blocking.
   */
  void poll(void);

  virtual void on_connection_closed() = 0;
  virtual boost::asio::io_service& get_io_service();

private:
  void connect_handler(boost::function< void(void) >,
		       boost::function< void(void) >,
		       const boost::system::error_code&);
  /**
   * Called when the server sends us a PING request. Sends a PONG back.
   */
  void ping_callback(Command*);

  boost::asio::io_service io_service;
};

#endif /*__CLIENT_HPP__ */
/**@}*/
