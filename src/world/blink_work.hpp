#ifndef BLINK_WORK_HPP_INCLUDED
#define BLINK_WORK_HPP_INCLUDED

#include <world/work.hpp>
#include <world/position.hpp>

class Entity;
class Mobility;
class Location;

// TODO use an internal pathwork to walk towards the goal, and actually
// blink when we are close enough.

class BlinkWork: public Work
{
public:
  BlinkWork(Entity* entity, const Position& position);
  ~BlinkWork() = default;
  bool tick(World* world) override final;

private:
  const Position destination;

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
