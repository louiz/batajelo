/**
 * @class RemoteGameClient
 */


#ifndef __REMOTE_GAME_CLIENT_HPP__
# define __REMOTE_GAME_CLIENT_HPP__

#include <functional>
#include <network/remote_client_base.hpp>

class GameServer;

class RemoteGameClient: public RemoteClientBase
{
public:
  RemoteGameClient(boost::asio::io_service& io_service);
  ~RemoteGameClient();
  void on_connection_closed() override final;
  void set_game_server(GameServer* server);

private:
  /**
   * Creates the default callbacks associated with a network command.
   * It is executed whenever that command is received.
   * See CommandHandler for details
   */
  void install_callbacks() override final;

  GameServer* server;
};

#endif
/**@}*/
