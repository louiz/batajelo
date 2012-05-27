#ifndef __SERVER_WORLD_HPP__
# define __SERVER_WORLD_HPP__

#include <world/world.hpp>

class ServerWorld: public World
{
public:
  ServerWorld();
  ~ServerWorld();
  /**
   * In addition to call World::init(), this will create the initial game
   * state.
   * The iniatial state is generated by associating some actions with the
   * turn number 1.
   * When a new occupant joins (even if the game is not started) it will
   * receive these commands as part of the "replay". If the game is not yet
   * started, the replay is only one turn long.
   * If a player joins later (the game is started), it will still receive
   * the actions associated with the first turn, plus the subsequent game
   * actions.
   */
  void init();
  /**
   * Called whenever we receive a MOVE command from one client.
   * We try to generate a path, if the move is valid, and then
   * send that path action to all clients.
   */
  void move_callback(Command*);

private:
  ServerWorld(const ServerWorld&);
  ServerWorld& operator=(const ServerWorld&);
};


#endif // __SERVER_WORLD_HPP__
