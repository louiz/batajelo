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
#include <functional>

#include <boost/asio.hpp>

#include <logging/logging.hpp>
#include <network/base_ioservice.hpp>
#include <network/base_socket.hpp>
#include <network/message.hpp>

/**
 * Does nothing, it is just used to exit the io_service.run_one() after
 * a timeout.
 */
static void poll_timeout_handler(const boost::system::error_code&)
{
}

template <class T>
class Server: public BaseIoservice, public BaseSocket
{
public:
  /**
   * Create the server instance. Use run() to start its loop.
   * @param port The port on which the servers accepts new connections.
   */
  Server(short port):
    BaseIoservice(),
    BaseSocket(io_service),
    port(port),
    timeout(io_service),
    acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
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
   * Starts the main loop, blocking.
   */
  void run()
  {
    this->io_service.run();
  }
  /**
   * Checks for network or timed events readiness.
   * The timeout argument makes this call block for that amount
   * of milliseconds.
   */
  void poll(long timeout = 0)
  {
    if (timeout == 0)
      {
        this->io_service.poll();
        return ;
      }
    if (this->timeout.expires_from_now(boost::posix_time::milliseconds(timeout)) == 0)
      // The last run_one() call returned because the timeout expired, so
      // we reinstall it. If that's not the case
      // (something actually happened on the socket)
      // we just need to reset the time of expiration, but not reinstall it.
      this->timeout.async_wait(&poll_timeout_handler);
    // Wait for one event to happen (either a timeout or something
    // on the socket).
    this->io_service.run_one();
    while (this->io_service.poll() != 0)
      ; // Execute all other available handlers, if any
  }
  /**
   * To be called by the a RemoteClient instance, to delete itself from
   * the RemoteClient list.
   */
  void remove_client(T* client)
  {
    typename std::vector<T*>::iterator it;
    for (it = this->clients.begin(); it < this->clients.end(); ++it)
      if (*it == client)
	{
	  this->clients.erase(it);
	  break ;
	}
    this->on_client_left(client);
    delete client;
  }
  /**
   * Search for a connected client with this login
   * @return RemoteClient* can be NULL if nothing is found
   */
  T* find_client_by_login(const std::string& login)
  {
    typename std::vector<T*>::iterator it;
    for (it = this->clients.begin(); it < this->clients.end(); ++it)
      if ((*it)->get_user() && (*it)->get_user()->get("login") == login)
	return *it;
    return 0;
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
    typename std::vector<unsigned long int>::iterator it;
    for (it = ids.begin(); it < ids.end(); ++it)
      {
	this->send_to_client(new Message(*message), *it);
      }
    // delete the message ourself, because we made a copy for each
    // client, and the original is still there and will not be deleted
    // by a client sending it (because it will not be sent).
    delete message;
  }

  /**
   * Sends a message to all clients.
   */
  void send_to_all_clients(Message* message)
  {
    typename std::vector<T*>::iterator it;
    for (it = this->clients.begin(); it < this->clients.end(); ++it)
      {
	(*it)->send(new Message(*message));
      }
    delete message;
  }

  /**
   * Sends a message to a client specified by its id.
   */
  void send_to_client(Message* message, unsigned long int id)
  {
    typename std::vector<T*>::iterator it;
    for (it = this->clients.begin(); it < this->clients.end(); ++it)
      {
    	if ((*it)->number == id)
	  {
	    (*it)->send(message);
	    return ;
	  }
      }
    // The message MUST be sent by now. If the corresponding client
    // was not in the list, something was wrong before calling this
    // function.
    assert(false);
  }

private:
  void install_accept_handler(void)
  {
    T* new_client = new T(this->io_service);

    this->acceptor.async_accept(new_client->get_socket(),
                                std::bind(&Server<T>::handle_accept,
                                          this, new_client,
                                          std::placeholders::_1));
  }

  void handle_accept(T* client, const boost::system::error_code& error)
  {
    if (error)
      {
	log_error("handle_accept failed: " << error);
	exit(1);
      }
    this->clients.push_back(client);
    this->on_new_client(client);
    this->install_accept_handler();
    client->start();
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

  std::vector<T*> clients;
  const short port;
  boost::asio::deadline_timer timeout;
  boost::asio::ip::tcp::acceptor acceptor;
};

#endif /*__SERVER_HPP__ */
/**#@}*/
