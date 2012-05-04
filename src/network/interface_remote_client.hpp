/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single remote client. It is kept in a list, by the Server
 * object. Using the CommandHandler interface, it executes callbacks
 * upon receiving command from the client, and sends commands when
 * we need to communicate something to it.
 * The derived class must install its own callback, install a transfer handler
 * in it or not, etc.
 * @class InterfaceRemoteClient
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __INTERFACE_REMOTE_CLIENT_HPP__
# define __INTERFACE_REMOTE_CLIENT_HPP__

#include <network/command_handler.hpp>
#include <network/command.hpp>
#include <network/timed_event_handler.hpp>
#include <network/timed_event.hpp>
#include <network/ping_handler.hpp>

using boost::asio::ip::tcp;

class InterfaceRemoteClient: public CommandHandler, public TimedEventHandler, public PingHandler
{
public:
  InterfaceRemoteClient(boost::asio::io_service&);
  ~InterfaceRemoteClient();
  /**
   * starts the client (install the read handler, etc)
   */
  void start();
  /**
   * @return tcp::socket&
   */
  tcp::socket& get_socket(void);
  /**
   * Send a ping request to the remote client.
   */
  void send_ping();
  /**
   * Called when the response to our ping request is received.
   */
  void on_pong(Command*);
  /**
   * The number of clients is incremented each time
   * a new client is accepted.
   */
  static unsigned long int clients_number;

  virtual void on_connection_closed() = 0;

  virtual boost::asio::io_service& get_io_service() = 0;

protected:
  const unsigned long int number;
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  virtual void install_callbacks() = 0;
  void install_read_handler(void);
};

#endif
/**@}*/
