#ifndef BLINK_WORK_HPP_INCLUDED
#define BLINK_WORK_HPP_INCLUDED

#include <world/ability_work.hpp>
#include <world/position.hpp>
#include <world/path_work.hpp>

class Entity;
class Mobility;
class Location;

// TODO use an internal pathwork to walk towards the goal, and actually
// blink when we are close enough.

class BlinkWork: public AbilityWork
{
public:
  BlinkWork(Entity* entity, const Position& position);
  ~BlinkWork() = default;
  bool tick(World* world) override final;

private:
  /**
   * The BlinkWork may use a PathWork to move the entity to a position close
   * enough for the actual blink to be executed.  Since there is a maximal
   * cast range, the entity needs to be move (using path_work) until we
   * reach a position where the distance between the Location and the
   * destination is < cast_distance.
   */
  PathWork path_work;
  const Position destination;
  static const Fix16 cast_distance;

  /**
   * The entity elements that we need to execute the work on.
   */
  Mobility* mobility;
  Location* location;

  BlinkWork(const BlinkWork&) = delete;
  BlinkWork(BlinkWork&&) = delete;
  BlinkWork& operator=(const BlinkWork&) = delete;
  BlinkWork& operator=(BlinkWork&&) = delete;
};


#endif /* BLINK_WORK_HPP_INCLUDED */
