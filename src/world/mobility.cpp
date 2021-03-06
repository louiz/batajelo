#include <world/world.hpp>
#include <world/position.hpp>

#include <world/mobility.hpp>
#include <world/location.hpp>

#include <logging/logging.hpp>

const ComponentType Mobility::component_type;

Mobility::Mobility(const Fix16& speed):
  speed(speed)
  last_movement{},
{
}

void Mobility::tick(Entity*, World*)
{
}

Fix16 Mobility::get_speed() const
{
  return this->speed;
}

void Mobility::set_speed(const Fix16& speed)
{
  this->speed = speed;
}

void Mobility::follow_path(Path& path, World* world, Location* location)
{
  if (path.empty())
    return;

  Position goal = path.front();
  Vec2 movement(goal - location->position());
  if (movement.length() == 0)
    {
      // Movement is too short to have something != 0 because of the Fix16 precision.
      // We consider to be at the goal.
      path.erase(path.begin());
      return ;
    }

  if (Position::distance(goal, location->position()) < this->speed)
    movement.set_length(Position::distance(goal, location->position()));
  else
    movement.set_length(this->speed);
  auto after_movement = location->position() + movement;

  // check for collision with other blocking entities
  auto number_of_rotations = 0;
  auto angle = 36;
  while (!location->is_position_valid(world, after_movement) && number_of_rotations < 360)
    {
      movement.rotate(angle);
      number_of_rotations += angle;
      after_movement = location->position() + movement;
    }
  if (number_of_rotations == 360)
    {
      log_debug("blocked");
      return;
    }
  this->last_movement = movement;
  location->position() = after_movement;
}

Fix16 Mobility::move_towards(const Position& goal, Location* location)
{
  Vec2 movement(goal - location->position());
  if (movement.length() == 0)
    {
      location->position() = goal;
      return 0;
    }

  if (Position::distance(goal, location->position()) < this->speed)
    movement.set_length(Position::distance(goal, location->position()));
  else
    movement.set_length(this->speed);
  location->position() += movement;
  this->last_movement = movement;
  return movement.length();
}

Fix16 Mobility::move_towards(const Position& goal, Location* location, Fix16 speed)
{
  auto speed_backup = this->get_speed();
  this->set_speed(speed);
  auto res = this->move_towards(goal, location);
  this->set_speed(speed_backup);
  return res;
}

Fix16 Mobility::get_angle() const
{
  auto res = this->last_movement.angle();
  if (res >= 0)
    return res;
  else
    return 360 + res;
}

Direction Mobility::get_direction() const
{
  auto angle = this->get_angle();
  static const auto wide = 22.5_fix;
  if (angle >= (360 - wide) ||
      angle < (0 + wide))
    return Direction::E;
  else if (angle >= (45 - wide) &&
      angle < (45 + wide))
    return Direction::SE;
  else if (angle >= (90 - wide) &&
      angle < (90 + wide))
    return Direction::S;
  else if (angle >= (135 - wide) &&
      angle < (135 + wide))
    return Direction::SW;
  else if (angle >= (180 - wide) &&
      angle < (180 + wide))
    return Direction::W;
  else if (angle >= (225 - wide) &&
      angle < (225 + wide))
    return Direction::NW;
  else if (angle >= (270 - wide) &&
      angle < (270 + wide))
    return Direction::N;
  else if (angle >= (315 - wide) &&
      angle < (315 + wide))
    return Direction::NE;
  assert(false);
}
