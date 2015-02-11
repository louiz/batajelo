#include <world/tasks/ability_task.hpp>
#include <world/abilities.hpp>
#include <world/entity.hpp>

AbilityTask::AbilityTask(Entity* entity):
  Task(entity),
  cast_point_reached(false)
{
  Abilities* abilities = entity->get<Abilities>();
  this->frontswing = abilities->cast_frontswing;
  this->backswing = abilities->cast_backswing;
}

bool AbilityTask::needs_to_be_restarted() const
{
  // If we were interrupted during the frontswing, we need to restart this
  // task later to actually cast the ability
  // If however we were in the backswing period (so cast_point has been
  // reached), we don't need to restart this Task
  return !this->cast_point_reached;
}
