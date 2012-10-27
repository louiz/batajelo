#include <logging/logging.hpp>
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
  SpawnWork* work = dynamic_cast<SpawnWork*>(w);
  assert(work);
  Unit* unit = world->create_unit(work->type_id);
  // find free spawning position
  unit->pos.x = this->x * CELL_SIZE;
  unit->pos.y = this->y * CELL_SIZE;
  world->insert_unit(unit);
  return true;
}

bool Building::set_rally_point(World* world, Work* w)
{
  RallyWork* work = dynamic_cast<RallyWork*>(w);
  assert(work);
  this->rally_point = work->position;
  return true;
}
