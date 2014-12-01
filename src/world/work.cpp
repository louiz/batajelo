#include <world/world.hpp>
#include <world/entity.hpp>
#include <world/unit.hpp>
#include <world/building.hpp>
#include <world/work.hpp>
#include <logging/logging.hpp>

PathWork::PathWork(Unit* unit, const Position& destination):
  path(),
  destination(std::move(destination)),
  unit(unit),
  calculated(false)
{
}

PathWork::~PathWork()
{
}

bool PathWork::tick(World* world)
{
  if (!this->calculated)
    {
      this->path = world->calculate_path(this->destination, this->unit);
      this->calculated = true;
    }
  this->unit->follow_path(this->path, world);
  if (this->calculated && this->path.empty())
    return true;
  return false;
}
