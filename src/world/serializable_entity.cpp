#include <world/serializable_entity.hpp>

SerializableEntity::SerializableEntity(const Entity& e)
{
  this->x = e.x.toString();
  this->y = e.y.toString();
  this->type_id = e.type_id;
}

SerializableEntity::~SerializableEntity()
{
}

SerializableEntity::SerializableEntity()
{
}
