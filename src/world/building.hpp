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

class Building: public Entity
{
public:
  Building();
  ~Building();
  Building(const Building&);
  bool contains(const Position&) const;
  void tick(World*);

  bool is_obstructing_position(Entity*, const Position&) const;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & name & type_id & x & y;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & name & type_id & x & y;
  }

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
};

#endif // __BUILDING_HPP__

