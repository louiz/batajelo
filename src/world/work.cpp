#include <world/entity.hpp>
#include <world/unit.hpp>
#include <world/building.hpp>
#include <world/work.hpp>

PathWork::PathWork(Unit* unit, Position pos):
  Work(boost::bind(&Unit::follow_path, unit, _1, _2)),
  path(0),
  end_position(pos),
  calculated(false)
{
}

PathWork::~PathWork()
{
}
