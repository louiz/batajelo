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
#include <mod/mod.hpp>
#include <network/server.hpp>
#include <network/remote_game_client.hpp>

class GameServer: public Server<RemoteGameClient>
{
public:
  GameServer(short port, Mod& mod);
  ~GameServer();
  ServerWorld* get_world() const;

  virtual void on_new_client(RemoteGameClient*);
  virtual void on_client_left(RemoteGameClient*);
  /**
   * Send the complete content of the replay contained in the World
   * to the client.
   */
  void send_replay(RemoteGameClient*);
  /**
   * Send a start command to the client telling it to start
   * confirming turns and actions from there.
   */
  void send_start_command(RemoteGameClient*);
  /**
   * Take all futur actions in the turn_handler, if they are not
   * completely validated yet, change their validations_needed value
   * to the current number of occupants, and send them to the client.
   */
  void send_and_adjust_future_commands(RemoteGameClient*);

  void adjust_and_revalidate_futur_commands();
  /**
   * Send a command to all clients saying that the action with this id
   * has been validated by all clients.
   */
  void send_ok(const unsigned int id, const unsigned long int by);
  /**
   * Send a command to all clients saying that the turn with this id
   * has been validated by all clients.
   */
  void send_turn(const unsigned int id, const unsigned long int by);

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
