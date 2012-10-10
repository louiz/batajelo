#include <world/building.hpp>

Building::Building()
{
}

Building::~Building()
{
}

Building::Building(const Building& o):
  cell_pos(o.cell_pos),
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
