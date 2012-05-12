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

#include <world/world.hpp>
#include <game/turn_handler.hpp>
#include <network/server.hpp>
#include <network/remote_game_client.hpp>

class GameServer: public TurnHandler, public Server<RemoteGameClient>
{
public:
  GameServer(short port):Server<RemoteGameClient>(port) { this->world = new World; }
  ~GameServer() {} ;
  World* get_world() const { return this->world; }

private:
  GameServer(const GameServer&);
  GameServer& operator=(const GameServer&);
  /**
   * The state of the world, synchronized through commands with the world
   * kept by each remote client.
   */
  World* world;
};

#endif // __GAME_SERVER_HPP__
/**#@}*/
