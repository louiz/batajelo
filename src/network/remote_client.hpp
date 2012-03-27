/** @addtogroup Network
 *  @{
 */

/**
 * Represents one single remote client. It is kept in a list, by the Server
 * object. Using the CommandHandler interface, it executes callbacks
 * upon receiving command from the client, and sends commands when
 * we need to communicate something to it.
 * @class RemoteClient
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __REMOTE_CLIENT_HPP__
# define __REMOTE_CLIENT_HPP__


#include <database/user.hpp>
#include <network/command_handler.hpp>


class Server;

using boost::asio::ip::tcp;

class RemoteClient: public CommandHandler
{
public:
  RemoteClient(boost::asio::io_service&, Server*);
  ~RemoteClient();
  /**
   * starts the client (install the read handler, etc)
   */
  void start();
  /**
   * @return tcp::socket&
   */
  tcp::socket& get_socket(void);
  /**
   * The number of clients is incremented each time
   * a new client is accepted.
   */
  static unsigned long int clients_number;

  virtual void on_connection_closed();
  User* get_user();
private:
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  void install_callbacks();
  void install_read_handler(void);
  void auth_callback(const std::string&);
  const unsigned long int number;
  /**
   * A pointer to the server, to call its method when the RemoteClient
   * has to be deleted.
   */
  Server* server;
  User* user;
};

#endif
/**@}*/
