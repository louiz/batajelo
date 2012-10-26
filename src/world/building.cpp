#include <world/building.hpp>
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
  height(height)
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

bool Building::spawn(World* world, Work* w)
{
  log_info("COCUOU je SPAWN");
  SpawnWork* work = static_cast<SpawnWork*>(w);
  Unit* unit = world->create_unit(work->type_id);
  // find free spawning position
  unit->pos.x = this->x * CELL_SIZE;
  unit->pos.y = this->y * CELL_SIZE;
  world->insert_unit(unit);
  return true;
}
