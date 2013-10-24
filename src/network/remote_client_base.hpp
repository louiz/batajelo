/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single remote client. It is kept in a list, by the Server
 * object. Using the CommandHandler interface, it executes callbacks
 * upon receiving command from the client, and sends commands when
 * we need to communicate something to it.
 * The derived classes must install their own callback, install a transfer handler
 * in it or not, etc.
 * @class RemoteClientBase
 */

#include <boost/asio.hpp>
#include <functional>

#ifndef REMOTE_CLIENT_BASE
# define REMOTE_CLIENT_BASE

#include <network/command_handler.hpp>
#include <network/command.hpp>
#include <network/timed_event_handler.hpp>
#include <network/timed_event.hpp>
#include <network/ping_handler.hpp>

class RemoteClientBase: public CommandHandler, public TimedEventHandler, public PingHandler
{
public:
  explicit RemoteClientBase(boost::asio::io_service&);
  ~RemoteClientBase();
  /**
   * starts the client (install the read handler, etc)
   */
  void start();
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
  /**
   * Returns the client number (aka id).
   */
  unsigned long int get_number() { return this->number; }
  /**
   * Returns the client number (aka id).
   */
  unsigned long int get_id() { return this->get_number(); }

  virtual void on_connection_closed() = 0;

protected:
  /**
   * The client number (aka id).
   */
  const unsigned long int number;
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  virtual void install_callbacks() = 0;
  void install_read_handler(void);
private:
  /**
   * We keep a reference on the io_service that was passed to us by the
   * Server, for convenience.
   */
  boost::asio::io_service& io_service;
};

#endif // REMOTE_CLIENT_BASE

/**@}*/