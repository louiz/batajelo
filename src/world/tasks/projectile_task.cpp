#include <world/tasks/projectile_task.hpp>
#include <world/world.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>

ProjectileTask::ProjectileTask(Entity* entity, std::weak_ptr<Entity> target):
  Task(entity),
  location(entity->get<Location>()),
  mobility(entity->get<Mobility>()),
  target(target),
  last_known_position(Position::zero)
{
}

bool ProjectileTask::tick(World* world)
{
  Position goal;
  if (!this->target.expired())
    {
      Location* location = this->target.lock()->get<Location>();
      assert(location);
      goal = location->position();
      this->last_known_position = goal;
    }
  else
    goal = this->last_known_position;
  this->mobility->move_towards(goal, this->location);
  if (goal == this->location->position())
    return true;
  return false;

}
