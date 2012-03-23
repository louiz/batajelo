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

#include <network/command_handler.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __REMOTE_CLIENT_HPP__
# define __REMOTE_CLIENT_HPP__

using boost::asio::ip::tcp;

class RemoteClient: public CommandHandler
{
public:
  RemoteClient(boost::asio::io_service&);
  ~RemoteClient();
  /**
   * starts the client (install the read handler, etc)
   */
  void start(void);
  /**
   * @return tcp::socket&
   */
  tcp::socket& get_socket(void);
  /**
   * The number of clients is incremented each time
   * a new client is accepted.
   */
  static int clients_number;

private:
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  void install_callbacks();
  void install_read_handler(void);
  void auth_callback(const std::string&);
  const int number;
};

#endif
/**@}*/
