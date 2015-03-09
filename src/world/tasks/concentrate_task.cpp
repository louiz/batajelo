#include <world/tasks/concentrate_task.hpp>
#include <logging/logging.hpp>

ConcentrateTask::ConcentrateTask(Entity* entity):
  Task(entity),
  quantity(0)
{
}

bool ConcentrateTask::tick(World* world)
{
  // TODO check that we do not overflow the Fix16 type
  this->quantity++;
  return false;
}

Fix16 ConcentrateTask::value() const
{
  return this->quantity;
}
