/**
 * @defgroup World
 * This group contains all elements representing the world or contained in it.
 * Units, building, etc. It is shared by the clients and game server and should
 * be 100% deterministic (no random, no float at all, etc), to be fully
 * synchronized everywhere.
 */

/** @addtogroup World
 *  @{
 */

/**
 * The world, contains the list of all entities, the game time, etc.
 */

#include <list>
#include <queue>

#ifndef __WORLD_HPP__
# define __WORLD_HPP__

#include <game/action.hpp>
#include <world/entity.hpp>
#include <world/occupant.hpp>
#include <network/command.hpp>

class World
{
public:
  World();
  ~World();
  /**
   * returns a pointer to the next entity of the world. If it returns NULL,
   * this means that the list is over and the next call will restart from the
   * beginning of the list.
   * If the list is empty, it will always return NULL.
   */
  Entity* get_next_entity();
  /**
   * Set the entity iterator at its beginning.
   */
  void reset_entity_iterator();
  /**
   * Insert an entity at the end of the list.
   */
  void insert_entity(Entity*);
  /**
   * Handle an user event that may generate a real action, or nothing.
   * The coordinates are the real-world ones.
   * (for example if the user gives on order while no unit is selected, nothing
   * happens).
   * If an action is generated with this event, generate_action() is called.
   */
  void handle_event(actions::Type type, unsigned int x, unsigned y);

  void generate_command(const char* name, const std::string& archive);
  Command* get_pending_command();

  void load_test();

  /**
   * Called whenever we receive a new_occupant message from the server.
   */
  void new_occupant_callback(Command* command);
  /**
   * Actually instert a occupant in the occupants list.
   */
  void add_new_occupant(Occupant*);

  /**
   * Called whenever we receive a occupant_left message from the server.
   */
  void occupant_left_callback(Command* command);
  /**
   * Actually remove the occupant from the occupants list.
   */
  void remove_occupant(Occupant*);

  /**
   * All the try_* methods are called ONLY by the server. It checks if the
   * action can be done etc, and it generate actions to be executed on the
   * server, and possibly on one or more clients.
   * (I think they should all be const.)
   */
  void try_move(Command*);

  /**
   * The list of other occupants of the game, when a new client connects to
   * the server, we had it to the list, when it disconnects we remove it.
   * Each occupant object contains an id corresponding to the
   * RemoteGameClient object stored in the server.
   */
  std::vector<Occupant*> occupants;

private:
  World(const World&);
  World& operator=(const World&);

  /**
   * The liste of all existing entities in the world.
   */
  std::list<Entity*> entities;
  /**
   * An iterator to help other classes get the entities one by
   * one without having to play with an iterator themselve, etc.
   */
  std::list<Entity*>::iterator entities_iterator;
  /**
   * The list of all action generated that needs to be sent to the
   * the remote server or the clients.
   * The server may generate actions too, in case of a game versus an IA or
   * something like that.
   */
  std::queue<Command*> commands_queue;
};

#endif // __WORLD_HPP__
/**@}*/
