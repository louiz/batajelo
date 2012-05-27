/** @addtogroup Network
 *  @{
 */

/**
 * The server running one game. It keeps one world representation, receives
 * command from various clients and sends back commands to execute, in the
 * futur, to concerned clients, and execute them later using the TurnHandler
 * class.
 * @class GameServer
 */

#ifndef __GAME_SERVER_HPP__
# define __GAME_SERVER_HPP__

#include <world/server_world.hpp>
#include <network/server.hpp>
#include <network/remote_game_client.hpp>

class GameServer: public Server<RemoteGameClient>
{
public:
  GameServer(short port);
  ~GameServer();
  ServerWorld* get_world() const;

  virtual void on_new_client(RemoteGameClient*);
  virtual void on_client_left(RemoteGameClient*);

  void tick();
  void pause_game();
  void unpause_game();
  void start_game();
  void send_pending_commands();

private:
  GameServer(const GameServer&);
  GameServer& operator=(const GameServer&);
  /**
   * The state of the world, synchronized through commands with the world
   * kept by each remote client.
   */
  ServerWorld* world;
};

#endif // __GAME_SERVER_HPP__
/**#@}*/
