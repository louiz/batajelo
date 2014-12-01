#include <logging/logging.hpp>
#include <world/building.hpp>
#include <world/unit.hpp>
#include <world/world.hpp>

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
  height(o.height),
  cells(o.cells)
{
}

bool Building::is_obstructing_position(const Unit* unit, const Position& position) const
{
  return false;
}

bool Building::contains(const Position&) const
{
  return false;
}

Position Building::get_center_position() const
{
  Position pos;
  pos.x = this->x * CELL_SIZE + ((this->cells * CELL_SIZE) / 2);
  pos.y = this->y * CELL_SIZE + ((this->cells * CELL_SIZE) / 2);
  return pos;
}
