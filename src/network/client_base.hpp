/** @addtogroup Network
 *  @{
 */

/**
 * The main network class for the clients.
 * Connects to the remote server, sends requests to it and
 * calls the appropriate callbacks when the answer is received (or
 * when a standalone message is received from the server)
 * @class ClientBase
 */

#include <functional>
#include <string>
#include <cstdlib>

#ifndef __CLIENT_BASE_HPP__
# define __CLIENT_BASE_HPP__

#include <network/base_ioservice.hpp>
#include <network/message_handler.hpp>
#include <network/ping_handler.hpp>
#include <network/timed_event_handler.hpp>
#include <network/message.hpp>
#include <network/timed_event.hpp>

#include <utils/time.hpp>

class ClientBase: public BaseIoservice, public MessageHandler,
                  public TimedEventHandler, public PingHandler
{
public:
  ClientBase();
  ~ClientBase();
  /**
   * Tries to connect to the remote server. Calls one of the given callbacks, if it
   * succeeds or fails.
   * @return void
   */
  void connect(const std::string&,
	       const short&,
	       std::function< void(void) > on_success = 0,
	       std::function< void(void) > on_failure = 0);
  /**
   * Install all the default callbacks.
   */
  virtual void install_callbacks() = 0;
  /**
   * Checks for network or timed events readiness.
   */
  void poll();

  virtual void on_connection_closed() = 0;

private:
  void connect_handler(std::function< void(void) >,
		       std::function< void(void) >,
		       const boost::system::error_code&);
  /**
   * Called when the server sends us a PING request. Sends a PONG back.
   */
  void ping_callback(Message*);
};

#endif /*__CLIENT_HPP__ */
/**@}*/
