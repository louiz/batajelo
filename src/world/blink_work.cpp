#include <world/blink_work.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/position.hpp>

#include <logging/logging.hpp>

#include <cassert>

BlinkWork::BlinkWork(Entity* entity, const Position& position):
  destination(position)
{
  this->mobility = entity->get<Mobility>();
  assert(this->mobility);
  this->location = entity->get<Location>();
  assert(this->location);
}

bool BlinkWork::tick(World* world)
{
  log_debug("blinking right now");
  this->location->position() = this->destination;
  return true;
}
