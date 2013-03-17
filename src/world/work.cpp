#include <world/entity.hpp>
#include <world/unit.hpp>
#include <world/building.hpp>
#include <world/work.hpp>

PathWork::PathWork(Unit* unit, const Position pos):
  Work(std::bind(&Unit::follow_path, unit, std::placeholders::_1, std::placeholders::_2)),
  path(),
  end_position(pos),
  calculated(false)
{}

RallyWork::RallyWork(Building* building, const Position pos):
  Work(std::bind(&Building::set_rally_point, building, std::placeholders::_1, std::placeholders::_2)),
  position(pos)
{}

BuildWork::BuildWork(Unit* unit, const unsigned short id, const short x, const short y):
  Work(std::bind(&Unit::build, unit, std::placeholders::_1, std::placeholders::_2)),
  id(id),
  x(x),
  y(y),
  building(nullptr)
{}

SpawnWork::SpawnWork(Building* building, const unsigned short id):
  Work(std::bind(&Building::spawn, building, std::placeholders::_1, std::placeholders::_2)),
  type_id(id),
  elapsed_time(0)
{}
