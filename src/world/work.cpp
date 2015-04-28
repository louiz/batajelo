#include <world/world.hpp>
#include <world/work.hpp>
#include <world/task.hpp>
#include <world/tasks/idle_task.hpp>
#include <world/world_callbacks.hpp>

Work::~Work()
{
}

void Work::set_task(std::unique_ptr<Task> task)
{
  this->world->callbacks->task_changed(this->entity, task.get());
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

const Task* Work::get_task() const
{
  return this->task.get();
}
