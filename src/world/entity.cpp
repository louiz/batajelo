#include <world/entity.hpp>
#include <cmath>
#include <world/world.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  type_id(Entity::current_type_id++)
{
  log_debug("Creating new unit model: id=" << this->type_id);
}

Entity::Entity(const Entity& model):
  id(++Entity::current_id),
  type_id(model.type_id)
{
}
