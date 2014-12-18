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

const Position& Location::position() const
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

bool Location::contains(const Position&) const
{
  return false;
}

bool Location::is_obstructing_position(const Position&) const
{
  return false;
}
