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

#ifndef __CLIENT_BASE_HPP__
# define __CLIENT_BASE_HPP__

#include <boost/algorithm/string.hpp>

#include <network/message_handler.hpp>
#include <network/ping_handler.hpp>
#include <network/timed_event_handler.hpp>
#include <network/message.hpp>
#include <network/timed_event.hpp>

#include <logging/logging.hpp>
#include <utils/time.hpp>

#include <functional>
#include <string>
#include <cstdlib>

class ClientBase: public MessageHandler, public TimedEventHandler,
                  public PingHandler
{
public:
  ClientBase() = default;
  ~ClientBase()
  {
    if (this->get_socket().is_open())
      {
        log_debug("Closing connection");
        this->get_socket().close();
      }
  }

  /**
   * Tries to connect to the remote server. Calls one of the given callbacks, if it
   * succeeds or fails.
   * @return void
   */
  void connect(const std::string& host,
	       const short& port,
	       std::function<void(void)> on_success=nullptr,
	       std::function<void(const boost::system::error_code&)> on_failure=nullptr)
  {
    // TODO use resolve and DNS
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    this->get_socket().async_connect(endpoint,
                                     std::bind(&ClientBase::connect_handler, this,
                                               on_success, on_failure,
                                               std::placeholders::_1));
    log_info("Connecting to " << host << ":" << port);
  }

  /**
   * Install all the default callbacks.
   */
  virtual void install_callbacks() = 0;
  /**
   * Checks for network or timed events readiness.
   */
  void poll()
  {
    while (IoService::get().poll() != 0)
      ;
  }

  virtual void on_connection_closed() = 0;

private:
  void connect_handler(std::function<void(void)> on_success,
		       std::function<void(const boost::system::error_code&)> on_failure,
		       const boost::system::error_code& error)
  {
    if (error)
      {
        log_info("Connection failed: " << error);
        if (on_failure)
          on_failure(error);
      }
    else
      {
        log_info("Connected.");
        this->install_callback("PING", std::bind(&ClientBase::ping_callback,
                                                 this, std::placeholders::_1));
        this->install_callbacks();
        this->install_read_handler();
        if (on_success)
          on_success();
      }
  }

  /**
   * Called when the server sends us a PING request. Sends a PONG back.
   */
  void ping_callback(Message*)
  {
    log_debug("Received PING");

    Message* message = new Message;
    message->set_name("PONG");
    this->send(message);
  }
};

#endif /*__CLIENT_HPP__ */
/**@}*/
