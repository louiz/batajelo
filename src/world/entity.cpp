#include <cmath>

#include <logging/logging.hpp>
#include <world/entity.hpp>
#include <world/world.hpp>
#include <world/work.hpp>

EntityId Entity::current_id = 0;
EntityType Entity::current_type_id = 0;

Entity::Entity():
  id(++Entity::current_id)
{
}

Entity::~Entity()
{
  this->clear_works();
}

void Entity::set_work(std::unique_ptr<Work>&& work)
{
  this->clear_works();
  this->works.push_back(std::move(work));
}

void Entity::queue_work(std::unique_ptr<Work>&& work)
{
  this->works.push_back(std::move(work));
}

void Entity::clear_works()
{
  this->works.clear();
}

void Entity::tick(World* world)
{
  for (const auto& ptr: this->components)
    {
      if (ptr)
        ptr->tick(this, world);
    }
  if (this->works.empty())
    return ;
  auto it = this->works.begin();
  if ((*it)->tick(world) == true)
    this->works.pop_front();
}
