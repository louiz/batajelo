#include <world/tasks/dash_task.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>

DashTask::DashTask(Entity* entity, const Position& goal, Fix16 speed, std::function<void(Entity*)> impact_cb,
                   std::function<void(void)> goal_cb):
  Task(entity),
  goal(goal),
  speed(speed),
  impact_callback(impact_cb),
  goal_callback(goal_cb),
  mobility(entity->get<Mobility>()),
  location(entity->get<Location>())
{
  assert(this->mobility);
  assert(this->location);
}

bool DashTask::tick(World* world)
{
  this->mobility->move_towards(this->goal, this->location, this->speed);
  // TODO check collision
  if (this->goal == this->location->position())
    {
      // TODO adjust location to be on a valid world position
      if (this->goal_callback)
        this->goal_callback();
      return true;
    }
  return false;
}
