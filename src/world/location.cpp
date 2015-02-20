#include <world/location.hpp>
#include <world/world.hpp>

#include <logging/logging.hpp>

const ComponentType Location::component_type;

Location::Location(const uint8_t width, const bool blocking):
  width(width),
  pos{},
  blocking(blocking)
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

void Location::set_blocking(const bool blocking)
{
  this->blocking = blocking;
}

bool Location::is_blocking() const
{
  return this->blocking;
}

bool Location::is_obstructing_position(const Position& pos,
                                       const Fix16& width) const
{
  if (!this->blocking)
    return false;

  auto distance = Position::distance(pos, this->pos);
  if (distance < width + this->get_width())
    return true;
  return false;
}

bool Location::is_position_valid(World* world, const Position& pos) const
{
  for (const auto& entity: world->entities)
    {
      Location* location = entity->get<Location>();
      if (!location || location == this)
        continue;
      if (location->is_obstructing_position(pos, this->width) == true)
        return false;
    }
  return true;
}
