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

#include <world/entity.hpp>
#include <game/action.hpp>

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
  /**
   * Create an action and put it in a list which will be sent by the client asap.
   * When the action is sent, it is removed from the list.
   * Note that these action are never executed directly on the world. We wait
   * for actions to be retrieved from the turn_handler (which contains actions
   * received from the server) and execute then only when it's time to.
   */
  void generate_action(const Action*);

  /**
   * Returns the next pending action to be sent on the network.
   * Returns NULL if there's no pending action.
   */
  const Action* get_pending_action();

  void load_test();

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
  std::queue<const Action*> actions_queue;
};

#endif // __WORLD_HPP__
/**@}*/
