#include <world/entity.hpp>
#include <cmath>
#include <world/world.hpp>
#include <world/work.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  type_id(Entity::current_type_id++)
{
  log_debug("Creating new unit model: id=" << this->type_id);
}

Entity::Entity(const Entity& model):
  id(++Entity::current_id),
  type_id(model.type_id),
  name(model.name)
{
}

Entity::~Entity()
{
  log_debug("Deleting entity");
  this->clear_works();
}

void Entity::set_work(Work* work)
{
  this->clear_works();
  this->works.push(work);
}

void Entity::queue_work(Work* work)
{
  this->works.push(work);
}

void Entity::clear_works()
{
  while (!this->works.empty())
    {
      delete this->works.front();
      this->works.pop();
    }
}
