#ifndef MOBILITY_COMPONENT_HPP_INCLUDED
#define MOBILITY_COMPONENT_HPP_INCLUDED

#include <world/components.hpp>
#include <fixmath/fix16.hpp>
#include <world/path.hpp>

#include <cstdint>

class Location;
class World;

class Mobility: public Component
{
public:
  static const ComponentType component_type = ComponentType::Mobility;

  Mobility(const Fix16& speed);
  ~Mobility() = default;
  void tick(Entity*, World*) override final;
  Fix16 get_speed() const;
  void set_speed(const Fix16& speed);
  /**
   * Given a path, change the location and cut down the path (if we moved
   * enough) to follow that path
   */
  void follow_path(Path& path, World* world, Location* location);
  /**
   * Move the location towards the given goal. Doesn't do any pathfinding or
   * collision handling, just moves unconditionally in the right direction.
   *
   * The goal is reached if after this function, the location's position == goal.
   */
  Fix16 move_towards(const Position& goal, Location* location);
  Fix16 move_towards(const Position& goal, Location* location, Fix16 speed);
  /**
   * Get the angle of the entity that has this mobility, based on the latest
   * movement done.
   */
  Fix16 get_angle() const;

private:
  Fix16 speed;
  Vec2 last_movement;

  Mobility(const Mobility&) = delete;
  Mobility(Mobility&&) = delete;
  Mobility& operator=(const Mobility&) = delete;
  Mobility& operator=(Mobility&&) = delete;
};


#endif /* MOBILITY_COMPONENT_HPP_INCLUDED */
