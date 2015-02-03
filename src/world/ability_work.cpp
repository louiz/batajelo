#include <world/ability_work.hpp>
#include <world/abilities.hpp>
#include <world/entity.hpp>

AbilityWork::AbilityWork(Entity* entity):
  Work(entity),
  cast_point_reached(false)
{
  Abilities* abilities = entity->get<Abilities>();
  this->frontswing = abilities->cast_frontswing;
  this->backswing = abilities->cast_backswing;
}
