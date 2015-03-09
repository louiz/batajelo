#include <world/world.hpp>
#include <world/work.hpp>
#include <world/task.hpp>
#include <world/world_callbacks.hpp>

void Work::set_task(World* world, std::unique_ptr<Task> task)
{
  world->callbacks->task_changed(this->entity, task->get_type());
  this->task = std::move(task);
}

void Work::interrupt()
{
  this->on_interrupted();
  this->task.reset(nullptr);
}

Task* Work::get_task()
{
  return this->task.get();
}
