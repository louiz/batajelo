#include <world/building.hpp>

Building::Building()
{
}

Building::~Building()
{
}

Building::Building(const Building& o):
  Entity(o),
  x(o.x),
  y(o.y),
  height(height)
{
}

void Building::tick(World*)
{
}

bool Building::is_obstructing_position(Entity* entity, const Position& position) const
{
  return false;
}

bool Building::contains(const Position&) const
{
  return false;
}
