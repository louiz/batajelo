#include <world/entity.hpp>

Entity::Entity():
  x(0),
  y(0),
  width(20),
  height(20),
  selected(false)
{
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
