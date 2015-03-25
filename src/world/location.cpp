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

Position Location::find_nearest_free_position(World* world, const Position& pos) const
{
  if (this->is_position_valid(world, pos))
    return pos;
  int distance = 20;
  int i = 1;
  Position res = pos;

  // that's ugly, but well
  while (true)
    {
      res.y = pos.y - distance*i;
      for (auto x = pos.x - (distance*i); x <= pos.x + (distance*i); x += distance)
        {
          res.x = x;
          if (this->is_position_valid(world, res))
            return res;
        }
      res.y = pos.y + distance*i;
      for (auto x = pos.x - (distance*i); x <= pos.x + (distance*i); x += distance)
        {
          res.x = x;
          if (this->is_position_valid(world, res))
            return res;
        }
      res.x = pos.x + distance*i;
      for (auto y = pos.y - (distance*i); y <= pos.y + (distance*i); y += distance)
        {
          res.y = y;
          if (this->is_position_valid(world, res))
            return res;
        }
      res.x = pos.x - distance*i;
      for (auto y = pos.y - (distance*i); y <= pos.y + (distance*i); y += distance)
        {
          res.y = y;
          if (this->is_position_valid(world, res))
            return res;
        }
      i++;
    }
}
