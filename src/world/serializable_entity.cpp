#include <world/serializable_entity.hpp>

SerializableEntity::SerializableEntity(const Entity& e)
{
  this->x = e.pos.x.toString();
  this->y = e.pos.y.toString();
  this->type_id = e.type_id;
  log_error("SerializableEntity: " << this->x << ":" << this->y);
}

SerializableEntity::~SerializableEntity()
{
}

SerializableEntity::SerializableEntity()
{
}
