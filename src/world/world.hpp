/**
 * @defgroup World
 * This group contains all elements representing the world or contained in
 * it (entities…).  It is shared by the clients and game server and should
 * be 100% deterministic (no random, no float at all, etc), to be fully
 * synchronized everywhere.
 */

/** @addtogroup World
 *  @{
 */

/**
 * The world, contains the list of all entities, the game time, etc.
 */

#ifndef __WORLD_HPP__
# define __WORLD_HPP__

#include <fixmath/fix16.hpp>

#include <list>
#include <memory>
#include <functional>

#include <world/abilities/ability.hpp>
#include <world/entity.hpp>
#include <world/entity_factory.hpp>

#include <world/rng.hpp>
#include <world/map.hpp>
#include <world/position.hpp>
#include <world/path.hpp>

class Team;
class Location;
class WorldCallbacks;

/**
 * From left to right, and from top to bottom, a cell has this size, in the
 * world representation. This means that if you add 100 to an entity
 * position, it will move to the next cell.
 */
static constexpr int16_t CELL_SIZE = 100;
static constexpr int16_t HALF_CELL_SIZE = CELL_SIZE / 2;
/**
 * A world-altitude of one represents this amount of pixels:
 */
#define LAYER_HEIGHT 24.f

class World
{
  friend class GameServer;
  friend class GameClient;
public:
  World();
  ~World();
  /**
   * Advance the world by the smallest step possible. Updates every entity,
   * timer, occupant etc it contains.
   */
  void tick();
  void pause();
  void unpause();
  void seed();
  unsigned long get_seed() const;
  void seed(unsigned long s);
  Path calculate_path(Position endpos, Location* location);
  Entity* do_new_entity(const EntityType type, const Position& pos,
                        const uint16_t team);
  void do_move(const std::vector<EntityId>& ids, const Position& pos,
               const bool queue);
  void do_follow(const std::vector<EntityId>& ids, const EntityId& target_id,
                 const bool queue);
  void do_cast_on_pos(const std::vector<EntityId>& ids, const Position& pos,
                      const AbilityType& type, const bool queue);
  void do_cast_on_target(const std::vector<EntityId>& ids,
                         const EntityId target_id, const AbilityType& type,
                         const bool queue);
  Entity* get_entity_by_id(EntityId id);
  std::shared_ptr<Entity> get_shared_entity_by_id(EntityId id);
  /**
   * Sends a message to the server saying that we confirm that action.
   */
  void confirm_action(const unsigned int);

  Map& get_map();
  /**
   * Sort the entities by their y position. This
   * is used to draw them in the correct order.
   */
  void sort_entities();
  /**
   * Gets the coordinate of the cell containing the given world position.
   */
  Cell get_cell_at_position(const Position& pos) const;
  /**
   * Returns the height (ingame) of the point at the given position.
   * It takes into account the highest level having a tile containing this
   * position and the tile height (see tile_heights in map.hpp)
   */
  Fix16 get_position_height(const Position& pos) const;
  /**
   * Return true if the given location can be seen by any entity of the given
   * team.
   */
  bool can_be_seen_by_team(const Position& position, const uint16_t team);

  Path smooth_path(CellPath path, Position& start,
                   const Position& end, const Fix16 width) const;
  /**
   * Returns wheither or not we can walk from the start position to the end
   * position, following a single straight line.
   */
  bool has_a_line_of_sight(const Position& start, const Position& end, const Fix16 step) const;
  /**
   * Returns whether the entity can walk in a straight line from one point
   * to another.  This is done by checking if two parallel lines of sight
   * exist, separated by the width of the entity.
   */
  bool can_walk_in_straight_line(const Position& start, const Position& end, const Fix16 step, const Fix16 width) const;
  /**
   * Returns whether or not we can move from one cell to another, according
   * to their respective heights.
   */
  bool can_traverse_cell(const short x, const short y,
                         const short x2, const short y2) const;
  /**
   * Return the position of the nearest corner of the given cell, using the next
   * cell to determine the optimal corner.
   */
  Position get_nearest_corner(const Position&, const CellIndex, const CellIndex,
                              const Fix16 width) const;
  /**
  * Return the position of the nearest corner of the given cell, in the case
  * where that is the last cell of the path.
  */
  Position get_nearest_corner(const Position&, const std::size_t, const Fix16 width) const;
  Position get_next_path_position(CellPath& path, const Position& current,
                                         const Position& end, const Fix16 width) const;

  std::unique_ptr<WorldCallbacks> callbacks;
  /**
   * The list of all existing entities in the world.
   */
  std::vector<std::shared_ptr<Entity>> entities;
  /**
   * List of newly created entities that needs to be added to the entities list at the end of the current tick();
   */
  std::vector<std::unique_ptr<Entity>> new_entities;

private:
  /**
   * Insert an entity at the end of the list.
   */
  void insert_entity(std::unique_ptr<Entity>&&);
  World(const World&);
  World& operator=(const World&);

  Map map;
  EntityFactory entity_factory;
  Rng rng;

public:
  // for debug
  CellPath current_path;
};

#endif // __WORLD_HPP__
/**@}*/
