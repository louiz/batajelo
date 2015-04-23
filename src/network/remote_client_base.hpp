/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single remote client. It is kept in a list, by the Server
 * object. Using the MessageHandler interface, it executes callbacks
 * upon receiving message from the client, and sends messages when
 * we need to communicate something to it.
 * The derived classes must install their own callback, install a transfer handler
 * in it or not, etc.
 * @class RemoteClientBase
 */

#ifndef REMOTE_CLIENT_BASE
# define REMOTE_CLIENT_BASE

#include <cstdint>

#include <functional>

#include <boost/asio.hpp>

#include <network/message_handler.hpp>
#include <network/message.hpp>
#include <network/timed_event_handler.hpp>
#include <network/timed_event.hpp>
#include <network/ping_handler.hpp>
#include <network/ioservice.hpp>

#include <logging/logging.hpp>

// In seconds
static constexpr int ping_interval = 10;

template <typename SocketType>
class RemoteClientBase: public MessageHandler<SocketType>, public TimedEventHandler, public PingHandler
{
public:
  template <typename... SocketArgs>
  RemoteClientBase(SocketArgs&&... socket_args):
    MessageHandler<SocketType>(std::forward<SocketArgs>(socket_args)...),
    id(RemoteClientBase<SocketType>::clients_number++)
  { }

  virtual ~RemoteClientBase() = default;

  /**
   * starts the client (install the read handler, etc)
   */
  virtual void start()
  {
    log_debug("Starting RemoteClientBase " << this->id);
    this->install_callbacks();
    this->install_timed_event(std::bind(&RemoteClientBase::send_ping, this), ping_interval);
    this->install_read_handler();
  }

  /**
   * Send a ping request to the remote client.
   */
  void send_ping()
  {
    Message* message = new Message;
    message->set_name("PING");
    this->request_answer(message, std::bind(&RemoteClientBase::on_pong, this, std::placeholders::_1), "PONG");
    this->ping_sent();
  }

  /**
   * Called when the response to our ping request is received.
   */
  void on_pong(Message*)
  {
    this->pong_received();
    log_debug("Current ping: " << this->get_latency() << "micro seconds.");
    this->install_timed_event(std::bind(&RemoteClientBase::send_ping, this), ping_interval);
  }

  /**
   * The number of clients is incremented each time
   * a new client is accepted.
   */
  static unsigned long int clients_number;

  /**
   * Returns the client number (aka id).
   */
  uint32_t get_id() const { return this->id; }

  boost::asio::ip::tcp::endpoint& get_endpoint()
  {
    return this->endpoint;
  }

protected:
  /**
   * The client number (aka id).
   */
  const uint32_t id;
  /**
   * Creates the default callbacks associated with a network message.
   * It is executed whenever that message is received.
   * See MessageHandler for details
   */
private:
  virtual void on_connection_closed() = 0;
  virtual void install_callbacks() = 0;
  /**
   * A endpoint containing the information of the remote peer.  It is set by
   * asio, when async_accept succeeds, just before the accept handler is
   * called.
   */
  boost::asio::ip::tcp::endpoint endpoint;
};

#endif // REMOTE_CLIENT_BASE

/**@}*/
