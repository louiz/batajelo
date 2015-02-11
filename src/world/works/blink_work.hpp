#ifndef BLINK_WORK_HPP_INCLUDED
#define BLINK_WORK_HPP_INCLUDED

#include <world/work.hpp>

#include <world/position.hpp>

/**
 * The BlinkWork may use a PathTask to move the entity to a position close
 * enough for the actual BlinkTask to be executed.  Since there is a maximal
 * cast range, the entity needs to be move until we reach a position where
 * the distance between the Location and the destination is < cast_distance.
 */

class Entity;
class Mobility;
class Location;

class BlinkWork: public Work
{
public:
  BlinkWork(Entity* entity, const Position& position);
  ~BlinkWork() = default;
  bool tick(World* world) override final;

private:
  const Position destination;
  static const Fix16 cast_distance;

  /**
   * Use this location to know if we are close enough to start the BlinkTask
   */
  Location* location;

  BlinkWork(const BlinkWork&) = delete;
  BlinkWork(BlinkWork&&) = delete;
  BlinkWork& operator=(const BlinkWork&) = delete;
  BlinkWork& operator=(BlinkWork&&) = delete;
};


#endif /* BLINK_WORK_HPP_INCLUDED */
