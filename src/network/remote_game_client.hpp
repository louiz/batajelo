/** @addtogroup Network
 *  @{
 */

/**
 * @class RemoteGameClient
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifndef __REMOTE_GAME_CLIENT_HPP__
# define __REMOTE_GAME_CLIENT_HPP__

#include <network/interface_remote_client.hpp>
#include <network/server.hpp>
#include <database/user.hpp>

using boost::asio::ip::tcp;

class RemoteGameClient: public InterfaceRemoteClient
{
public:
  RemoteGameClient(boost::asio::io_service&, Server<RemoteGameClient>*);
  ~RemoteGameClient();
  virtual void on_connection_closed();
  virtual boost::asio::io_service& get_io_service();

private:
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  virtual void install_callbacks();
  void ok_callback(Command* command);
  void send_ok(const unsigned int id, const unsigned long int by);
  /**
   * A pointer to the server, to call its method when the RemoteGameClient
   * has to be deleted.
   */
  Server<RemoteGameClient>* server;
};

#endif
/**@}*/
