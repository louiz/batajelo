/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single remote client connected to the main server (the one
 * doing authentication, history, chat, etc).
 * @class RemoteClient
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __REMOTE_CLIENT_HPP__
# define __REMOTE_CLIENT_HPP__

#include <network/interface_remote_client.hpp>
#include <network/transfer_sender.hpp>
#include <network/server.hpp>
#include <database/user.hpp>

using boost::asio::ip::tcp;

class RemoteClient: public InterfaceRemoteClient
{
public:
  RemoteClient(boost::asio::io_service&, Server<RemoteClient>*);
  ~RemoteClient();
  virtual void on_connection_closed();

  virtual boost::asio::io_service& get_io_service();
  /**
   * Sends a file to the remote client.
   * @param filename The file to send.
   */
  void send_file(const std::string&);
  /**
   * Sends a ping request to the remote client.
   */
  void send_ping();
  /**
   * Called when we receive the response to our ping.
   */
  void on_pong(Command*);
  /**
   * To be called whenever a file transfer ends.
   * Removes the TransferSender from the list.
   * @param transfer The TransferSender to remove from the list.
   */
  void on_transfer_ended(const TransferSender*);
  /**
   * Called when the client is successfully authenticated.
   * For example, checks if there are news to send, or offline messages, etc
   */
  void on_auth_success();
  /**
   * Return a pointer on the user associated with this client.
   * If it's not null, the client is actually authenticated as this user.
   */
  User* get_user();

private:
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  virtual void install_callbacks();
  void auth_callback(Command*);
  void transfer_callback(Command*);
  /**
   * A pointer to the server, to call its method when the RemoteClient
   * has to be deleted.
   */
  Server<RemoteClient>* server;
  User* user;
  /**
   * A list of all the current file transfers with the client.
   */
  std::vector<TransferSender*> senders;
};

#endif
/**@}*/
