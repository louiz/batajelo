/** @addtogroup Network
 *  @{
 */

/**
 * The Server is the part doing the accept on a socket and spawning one
 * "remote client" each time, and adding them to a list. Each client will
 * then read and write on their own socket.
 * The server is specified using the type of the remote client. Either this is
 * a RemoteClient and we then do authentication, we handle a database, friends,
 * chat, statistics, history etc. Or this is a GameRemoteClient and we just handle
 * one single game.
 * @class Server
 */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#include <istream>
#include <vector>
#include <memory>
#include <functional>

#include <signal.h>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>

#include <logging/logging.hpp>
#include <network/tcp_socket.hpp>
#include <network/message.hpp>
#include <utils/time.hpp>

template <class T>
class Server: public TCPSocket
{
public:
  /**
   * Create the server instance. Use run() to start its loop.
   * @param port The port on which the servers accepts new connections.
   */
  Server(short port):
    TCPSocket(),
    port(port),
    timeout(IoService::get()),
    acceptor(IoService::get(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    accepting_client(nullptr)
  {
  }
  virtual ~Server()
  {
    log_debug("closing socket");
  }
  /**
   * Starts the server
   */
  void start()
  {
    this->accept();
  }

  /**
   * Checks for network or timed events readiness.
   * The timeout argument makes this call block for that amount
   * of milliseconds.
   */
  void poll(long t)
  {
    if (t == 0)
      {
        while (IoService::get().poll())
          ;
        return ;
      }

    if (this->timeout.expires_from_now(boost::asio::steady_timer::duration(t)) == 0)
      // The last run_one() call returned because the timeout expired, so
      // we reinstall it. If that's not the case
      // (something actually happened on the socket)
      // we just need to reset the time of expiration, but not reinstall it.
      this->timeout.async_wait([](const boost::system::error_code&){});
    // Wait for one event to happen (either a timeout or something
    // on the socket).
    IoService::get().run_one();
    while (IoService::get().poll() != 0)
      ; // Execute all other available handlers, if any
  }
  /**
   * To be called by the a RemoteClient instance, to delete itself from
   * the RemoteClient list.
   */
  void remove_client(T* client)
  {
    this->on_client_left(client);

    auto it = std::find_if(this->clients.begin(), this->clients.end(),
                        [client](const std::unique_ptr<T>& c) -> bool
                        {
                          return c.get() == client;
                        });
    assert(it != this->clients.end());
    this->clients.erase(it);
  }
  /**
   * Search for a connected client with this login
   * @return RemoteClient* can be NULL if nothing is found
   */
  T* find_client_by_login(const std::string& login)
  {
    for (const auto& client: this->clients)
      if (client->get_user() && client->get_user()->get("login") == login)
	return client.get();
    return nullptr;
  }

  /**
   * Called after a client just connected to us.
   */
  virtual void on_new_client(T*)
  {
  }
  /**
   * Called after the client has been disconnected.
   * Must NOT try to send anything to it, and should not assume
   * it's still in the clients' list.
   */
  virtual void on_client_left(T*)
  {
  }

  /**
   * Sends a message to the list of the given remote clients (using their id).
   */
  void send_to_list_of_clients(Message* message,
			       std::vector<unsigned long int> ids)
  {
    for (const auto& id: ids)
      this->send_to_client(new Message(*message), id);
    // delete the message ourself, because we made a copy for each
    // client, and the original is still there and will not be deleted
    // by a client sending it.
    delete message;
  }

  /**
   * Sends a message to all clients.
   */
  void send_to_all_clients(Message* message)
  {
    for (const auto& client: this->clients)
      client->send(new Message(*message));
    delete message;
  }

  /**
   * Sends a message to a client specified by its id.
   */
  void send_to_client(Message* message, unsigned long int id)
  {
    for (const auto& client: this->clients)
      if (client->number == id)
        {
          client->send(message);
          return ;
        }
    // The message MUST be sent by now. If the corresponding client
    // was not in the list, something was wrong before calling this
    // function.
    assert(false);
  }

  bool is_started() const
  {
    return this->started;
  }

private:
  void install_accept_handler(void)
  {
    this->accepting_client = std::make_unique<T>();

    this->acceptor.async_accept(this->accepting_client->get_socket(),
                                std::bind(&Server<T>::handle_accept,
                                          this, std::placeholders::_1));
  }

  void handle_accept(const boost::system::error_code& error)
  {
    if (error)
      {
	log_error("handle_accept failed: " << error);
	exit(1);
      }
    auto new_client = this->accepting_client.get();
    this->clients.push_back(std::move(this->accepting_client));
    this->on_new_client(new_client);
    new_client->start();

    this->install_accept_handler();
  }

  void accept(void)
  {
    boost::system::error_code error;
    error = this->acceptor.listen(512, error);
    if (error)
      {
	log_error("Error on listen() [" << error << "]. Exiting.");
	return;
      }
    this->install_accept_handler();
  }

protected:
  std::vector<std::unique_ptr<T>> clients;
private:
  const short port;
  boost::asio::steady_timer timeout;
  boost::asio::ip::tcp::acceptor acceptor;
  /**
   * This client is created BEFORE we accept a new connection. When a new
   * client is accepted, the new socket is inserted in it, and we can then
   * insert that client in the clients vector.
   */
  std::unique_ptr<T> accepting_client;
};

#endif /*__SERVER_HPP__ */
/**#@}*/
