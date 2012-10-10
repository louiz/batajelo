#ifndef __SERVER_WORLD_HPP__
# define __SERVER_WORLD_HPP__

#include <world/world.hpp>
#include <mod/mod.hpp>

class ServerWorld: public World
{
public:
  ServerWorld(Mod&);
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
  /**
   * Called whenever we receive a BUILD command from one client.
   * Check if the position is valid and the building can be built (money, the unit actually has this ability, etc).
   */
  void build_callback(Command*);
  bool validate_action(const unsigned int id, const unsigned long int by);
  bool validate_turn(const unsigned int id, const unsigned long int by);
  /**
   * Generate a unit of the given type, at the given position and adds the
   * event of its creation to the first turn of the replay (which will be
   * sent to any joining client).
   */
  void spawn_unit(const size_t, const int, const int);

private:
  ServerWorld(const ServerWorld&);
  ServerWorld& operator=(const ServerWorld&);
};

#endif // __SERVER_WORLD_HPP__
