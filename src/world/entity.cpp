#include <cmath>

#include <logging/logging.hpp>
#include <world/entity.hpp>
#include <world/world.hpp>
#include <world/work.hpp>

EntityId Entity::current_id = 0;
EntityType Entity::current_type_id = 0;

Entity::Entity():
  // type_id(Entity::current_type_id++)
  id(++Entity::current_id)

{
}

Entity::Entity(const Entity& model):
  id(++Entity::current_id),
  type_id(model.type_id),
  name(model.name)
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
  for (auto it = this->components.begin();
       it != this->components.end(); ++it)
    {
      it->second->tick(this, world);
    }
  if (this->works.empty())
    return ;
  auto it = this->works.begin();
  if ((*it)->tick(world) == true)
    this->works.pop_front();
}
