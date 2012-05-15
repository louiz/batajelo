#include <world/entity.hpp>

unsigned short Entity::current_id = 0;
unsigned short Entity::current_type_id = 0;

Entity::Entity():
  x(0),
  y(0),
  width(20),
  height(20),
  selected(false),
  type_id(++Entity::current_type_id)
{
}

Entity::Entity(const Entity& e):
  id(++Entity::current_id)
{
  this->y = e.y;
  this->x = e.x;
  this->width = e.width;
  this->height = e.height;
  this->selected = e.selected;
  this->type_id = e.type_id;
  log_debug("Creating new unit of id: " << this->id);
}

Entity::~Entity()
{
}

bool Entity::is_selected() const
{
  return this->selected;
}

bool Entity::contains(unsigned int x, unsigned int y) const
{
  log_debug(x << ":" << y);
  if ((x >= this->x) && (y >= this->y) &&
      (x <= this->x + this->width) && (y <= this->y + this->height))
    return true;
  return false;
}
