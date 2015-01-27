#include <world/blink_work.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/position.hpp>

#include <logging/logging.hpp>

#include <cassert>

const Fix16 BlinkWork::cast_distance = 600;

BlinkWork::BlinkWork(Entity* entity, const Position& position):
  path_work(entity, position),
  destination(position)
{
  this->mobility = entity->get<Mobility>();
  assert(this->mobility);
  this->location = entity->get<Location>();
  assert(this->location);
}

bool BlinkWork::tick(World* world)
{
  auto distance = Position::distance(this->location->position(), this->destination);
  log_debug("Distance to destination: " << distance);
  if (distance > this->cast_distance)
    {
      this->path_work.tick(world);
      return false;
    }
  log_debug("blinking right now");
  this->location->position() = this->destination;
  return true;
}
