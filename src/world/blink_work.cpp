#include <world/blink_work.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/position.hpp>
#include <world/world_callbacks.hpp>

#include <logging/logging.hpp>

#include <cassert>

const Fix16 BlinkWork::cast_distance = 600;

BlinkWork::BlinkWork(Entity* entity, const Position& position):
  AbilityWork(entity),
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
  if (!this->cast_point_reached &&
      Position::distance(this->location->position(), this->destination) > this->cast_distance)
    {
      this->path_work.tick(world);
      return false;
    }
  if (this->frontswing)
    {
      this->frontswing--;
      return false;
    }
  if (!this->cast_point_reached)
    {
      log_debug("blinking right now");
      if (world->callbacks->ability_casted)
        {
          world->callbacks->ability_casted(this->entity, AbilityType::Blink,
                                           nullptr, this->destination);
        }
      this->location->position() = this->destination;
      this->cast_point_reached = true;
    }
  if (this->backswing)
    {
      this->backswing--;
      return false;
    }
  return true;
}
