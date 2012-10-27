#include <cmath>

#include <logging/logging.hpp>
#include <world/entity.hpp>
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
  if (this->works.empty())
    return ;
  Work* current = this->works.front();
  this->works.pop(); // pop the current without deleting it, in case we want
                     // to add it back.
  while (!this->works.empty())
    {
      delete this->works.front();
      this->works.pop();
    }
  // if the current work is not interruptible, we put it back in the queue,
  // as if we didn’t removed it.
  if (!current->is_interruptible())
    this->works.push(current);
  else
    delete current;
}

void Entity::tick(World* world)
{
  if (this->works.empty())
    return ;
  Work* current_work = this->works.front();
  if ((*current_work)(world, current_work) == true)
    {
      delete current_work;
      this->works.pop();
    }
}
