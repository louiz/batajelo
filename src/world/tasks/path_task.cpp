#include <world/tasks/path_task.hpp>

#include <world/world.hpp>
#include <world/entity.hpp>
#include <world/path.hpp>
#include <world/mobility.hpp>
#include <world/location.hpp>

#include <logging/logging.hpp>

#include <cassert>

PathTask::PathTask(Entity* entity, const Position& destination):
  Task(entity),
  path(),
  destination(destination),
  calculated(false),
  mobility(entity->get<Mobility>()),
  location(entity->get<Location>())
{
  assert(this->mobility);
  assert(this->location);
}

bool PathTask::tick(World* world)
{
  if (!this->calculated)
    {
      this->path = world->calculate_path(this->destination, this->location);
      this->calculated = true;
    }
  this->mobility->follow_path(this->path, world, this->location);
  if (this->calculated && this->path.empty())
    return true;
  return false;
}
