/** @addtogroup World
 *  @{
 */

/**
 * An entity that cannot move, takes one or more map cell and can cast
 * spells.
 *  @class Building
 */

#ifndef __BUILDING_HPP__
# define __BUILDING_HPP__

#include <world/entity.hpp>
#include <world/work.hpp>

class Unit;

class Building: public Entity
{
public:
  Building();
  ~Building();
  Building(const Building&);
  bool contains(const Position&) const;

  bool is_obstructing_position(const Unit*, const Position&) const;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & name & type_id & x & y;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & name & type_id & x & y;
  }

  bool spawn(World*, Work*);
  bool set_rally_point(World*, Work*);
  Position get_center_position() const;
  /**
   * return a list of type_id of the units currently waiting to be spawned.
   */
  std::vector<unsigned short> get_spawning_queue() const;
  /**
   * Return the elapsed time for the currently spawning work.
   */
  unsigned int get_spawn_time_elapsed() const;

private:
  Building& operator=(const Building&);

public:
  /**
   * The width of the building, in cells (can be one, two, etc)
   */
  short cells;
  /**
   * The top left cell where the building is placed.
   */
  short x;
  short y;
  /**
   * The height
   */
  int16_t height;
  /**
   * The entity health.
   */
  uint health;
  /**
   * The time needed to build it
   */
  unsigned int build_time;
  /**
   * The rally point
   */
  Position rally_point;
};

#endif // __BUILDING_HPP__

