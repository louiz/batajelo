#include <world/tasks/blink_task.hpp>

#include <world/world.hpp>
#include <world/world_callbacks.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>

#include <logging/logging.hpp>
#include <cassert>

BlinkTask::BlinkTask(Entity* entity, const Position& destination):
  AbilityTask(entity),
  destination(destination),
  mobility(entity->get<Mobility>()),
  location(entity->get<Location>())
{
  assert(this->mobility);
  assert(this->location);
}

bool BlinkTask::tick(World* world)
{
  if (this->frontswing)
    {
      this->frontswing--;
      return false;
    }
  if (!this->cast_point_reached)
    {
      log_debug("blinking right now");
      world->callbacks->ability_casted(this->entity, AbilityType::Blink,
                                       nullptr, this->destination);
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
