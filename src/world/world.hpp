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

#ifndef __WORLD_HPP__
# define __WORLD_HPP__

#include <world/entity.hpp>

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

  void load_test();

private:
  World(const World&);
  World& operator=(const World&);

  std::list<Entity*> entities;
  std::list<Entity*>::iterator entities_iterator;
};

#endif // __WORLD_HPP__
/**@}*/
