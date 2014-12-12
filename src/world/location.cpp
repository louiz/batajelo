#include <world/location.hpp>

const ComponentType Location::component_type;

Location::Location(const uint8_t width):
  width(width),
  pos{}
{
}

void Location::tick(Entity*, World*)
{
}

Position& Location::position()
{
  return this->pos;
}

Fix16 Location::get_width() const
{
  return this->width;
}

void Location::set_width(const Fix16& width)
{
  this->width = width;
}
