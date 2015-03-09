#include <world/world.hpp>
#include <world/position.hpp>

#include <world/mobility.hpp>
#include <world/location.hpp>

#include <logging/logging.hpp>

const ComponentType Mobility::component_type;

Mobility::Mobility(const Fix16& speed):
  speed(speed)
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
  Position goal = path.front();
  Vec2 movement(goal - location->position());
  if (movement.length() == 0)
    {
      // Movement is too short to have something != 0 because of the Fix16 precision.
      // We consider to be at the goal.
      path.pop_front();
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
  location->position() = after_movement;
}

void Mobility::move_towards(const Position& goal, Location* location)
{
  Vec2 movement(goal - location->position());
  if (movement.length() == 0)
    {
      location->position() = goal;
      return;
    }

  if (Position::distance(goal, location->position()) < this->speed)
    movement.set_length(Position::distance(goal, location->position()));
  else
    movement.set_length(this->speed);
  location->position() += movement;
  this->last_movement = movement;
}

void Mobility::move_towards(const Position& goal, Location* location, Fix16 speed)
{
  auto speed_backup = this->get_speed();
  this->set_speed(speed);
  this->move_towards(goal, location);
  this->set_speed(speed_backup);
}
Fix16 Mobility::get_angle() const
{
  return this->last_movement.angle();
}
