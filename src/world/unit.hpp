/** @addtogroup World
 *  @{
 */

/**
 * Just an Entity that can move itself independently, cast spell, be crated by buildings (or some special spells)
 * @class Unit
 */

#ifndef __UNIT_HPP__
# define __UNIT_HPP__

#include <world/entity.hpp>
#include <world/path.hpp>
#include <world/position.hpp>

class Work;

class Unit: public Entity
{
public:
  Unit();
  ~Unit();
  Unit(const Unit&);
  bool contains(const Position&) const;

  bool follow_path(World*, Work*);
  bool build(World*, Work*);
  void steer_to_avoid_obstacle(Vec2&, World*) const;
  /**
   * Returns a pointer to the entity just ahead of us (that means that if we
   * follow the givement vector movement, we will collide with it). If
   * there's none, NULL is returned.
   */
  Unit* get_unit_ahead(const Vec2&, World*) const;
  /**
   * Return true if we will collide with a wall OR a unit if we move
   * following that vector.
   */
  bool will_collide_if_we_move(Vec2&, World*) const;
  /**
   * Returns weither or not this entity will go into a wall if we move on
   * following the given movement.
   */
  bool is_wall_ahead(Vec2&, World*) const;
  /**
   * Return whether or not the unit can be pushed by an other entity.
   Mostly, if it is not doing any action and is not on Hold position, it
   can.
   */
  bool can_be_pushed() const;
  bool is_obstructing_position(Entity*, const Position&) const;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & type_id & pos;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & type_id & pos;
  }


private:
  Unit& operator=(const Unit&);

public:
  Position pos;
  /**
   * The width
   */
  short width;
  /**
   * The height
   */
  int16_t height;
  /**
   * The entity health.
   */
  uint health;
  /**
   * The entity speed
   */
  mpreal speed;
};


#endif // __UNIT_HPP__

