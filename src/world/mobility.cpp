#include <world/world.hpp>
#include <world/position.hpp>

#include <world/mobility.hpp>
#include <world/location.hpp>

const ComponentType Mobility::component_type;

const std::vector<ComponentType> Mobility::dependencies = {ComponentType::Location};

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
  location->position() += movement;
}
