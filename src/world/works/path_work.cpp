#include <world/works/path_work.hpp>

#include <world/tasks/path_task.hpp>

#include <world/world.hpp>
#include <world/entity.hpp>

PathWork::PathWork(Entity* entity, const Position& destination):
  Work(entity),
  destination(destination)
{
}

bool PathWork::tick(World* world)
{
  if (!this->task)
    this->set_task(world,
                   std::make_unique<PathTask>(this->entity, this->destination));
  return this->task->tick(world);
}
