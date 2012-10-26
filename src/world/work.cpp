#include <world/entity.hpp>
#include <world/unit.hpp>
#include <world/building.hpp>
#include <world/work.hpp>

PathWork::PathWork(Unit* unit, Position pos):
  Work(boost::bind(&Unit::follow_path, unit, _1, _2)),
  path(0),
  end_position(pos),
  calculated(false)
{}


BuildWork::BuildWork(Unit* unit, const unsigned short id, const short x, const short y):
  Work(boost::bind(&Unit::build, unit, _1, _2)),
  id(id),
  x(x),
  y(y),
  building(0)
{}

SpawnWork::SpawnWork(Building* building, const unsigned short id):
  Work(boost::bind(&Building::spawn, building, _1, _2)),
  type_id(id),
  elapsed_time(0)
{}
