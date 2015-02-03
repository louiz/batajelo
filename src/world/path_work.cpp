#include <logging/logging.hpp>

#include <world/path_work.hpp>
#include <world/world.hpp>
#include <world/entity.hpp>

#include <world/mobility.hpp>
#include <world/location.hpp>

PathWork::PathWork(Entity* entity, const Position& destination):
  Work(entity),
  path(),
  destination(destination),
  calculated(false)
{
  this->mobility = entity->get<Mobility>();
  assert(this->mobility);
  this->location = entity->get<Location>();
  assert(this->location);
}

PathWork::~PathWork()
{
}

bool PathWork::tick(World* world)
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
