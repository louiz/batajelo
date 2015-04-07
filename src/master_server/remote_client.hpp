/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single remote client connected to the main server (the one
 * doing authentication, history, chat, etc).
 * @class RemoteClient
 */

#ifndef __REMOTE_CLIENT_HPP__
# define __REMOTE_CLIENT_HPP__

#include <boost/asio.hpp>
#include <functional>
#include "batadb.hpp"

#include <network/remote_client_base.hpp>

class MasterServer;
class Message;
class TransferSender;

class RemoteClient: public RemoteClientBase
{
public:
  RemoteClient();
  ~RemoteClient();
  void on_connection_closed() override final;
  void set_server(MasterServer* server);

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
  void on_pong(Message*);
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
  const db::User* get_user() const;

private:
  /**
   * Keep track of which user is associated with this client. Used to get
   * various other information from the database (the contact list, account
   * informations, etc).
   * If it's nullptr, the user did not successfully log in (yet)
   */
  std::unique_ptr<db::User> user;
  /**
   * Creates the default callbacks associated with a network message.
   * It is executed whenever that message is received.
   * See MessageHandler for details
   */
  virtual void install_callbacks();
  void auth_callback(Message*);
  void transfer_callback(Message*);
  void on_user_logged_in();
  /**
   * A pointer to the server owning use
   */
  MasterServer* server;
  /**
   * A list of all the current file transfers with the client.
   */
  std::vector<TransferSender*> senders;
};

#endif
/**@}*/
