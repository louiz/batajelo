#include <cmath>

#include <logging/logging.hpp>
#include <world/entity.hpp>
#include <world/world.hpp>
#include <world/work.hpp>
#include <world/tasks/idle_task.hpp>
#include <world/world_callbacks.hpp>

EntityId Entity::current_id = 0;

Entity::~Entity()
{
  this->clear_works();
  log_debug("destroyed entity: " << this->get_id());
}

void Entity::set_work(std::unique_ptr<Work> work)
{
  this->clear_works();
  this->works.push_back(std::move(work));
}

void Entity::queue_work(std::unique_ptr<Work> work)
{
  this->works.push_back(std::move(work));
}

void Entity::clear_works()
{
  this->works.clear();
}

void Entity::interrupt()
{
  if (this->works.empty())
    return;
  this->works.front()->interrupt();
}

Work* Entity::get_current_work()
{
  if (this->works.empty())
    return nullptr;
  return this->works.front().get();
}

const Work* Entity::get_current_work() const
{
  if (this->works.empty())
    return nullptr;
  return this->works.front().get();
}

void Entity::tick(World* world)
{
  for (const auto& stat: this->status)
    {
      stat->tick();
    }

  for (const std::unique_ptr<Component>& ptr: this->components)
    {
      if (ptr)
        ptr->tick(this, world);
    }
  if (this->works.empty())
    return ;
  auto& work = this->works.front();
  if (work->tick() == true)
    this->works.pop_front();
  if (works.empty())
    world->callbacks->task_changed(this, &IdleTask::that);
}

void Entity::kill()
{
  this->to_be_deleted = true;
}

bool Entity::is_dead() const
{
  return this->to_be_deleted;
}

void Entity::make_manipulable()
{
  this->manipulable = true;
}

bool Entity::is_manipulable() const
{
  return this->manipulable;
}
