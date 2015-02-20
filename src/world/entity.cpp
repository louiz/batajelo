#include <cmath>

#include <logging/logging.hpp>
#include <world/entity.hpp>
#include <world/world.hpp>
#include <world/work.hpp>

EntityId Entity::current_id = 0;

Entity::Entity(const EntityType& type):
  id(++Entity::current_id),
  type(type),
  to_be_deleted(false),
  manipulable(false)
{
}

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
  if (work->tick(world) == true)
    this->works.pop_front();
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
