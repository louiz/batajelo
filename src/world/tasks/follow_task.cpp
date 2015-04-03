#include <world/tasks/follow_task.hpp>

#include <world/world.hpp>
#include <world/entity.hpp>
#include <world/mobility.hpp>
#include <world/location.hpp>

#include <cassert>

#include <logging/logging.hpp>

FollowTask::FollowTask(Entity* entity, const std::weak_ptr<Entity>& target):
  Task(entity),
  target(target),
  current_destination(Position::invalid),
  path{},
  mobility(entity->get<Mobility>()),
  location(entity->get<Location>())
{
  assert(this->mobility);
  assert(this->location);
}

bool FollowTask::tick(World* world)
{
  if (this->target.expired())
    return true;

  // TODO check if the target is visible by the entity's team

  Location* target_location = this->target.lock()->get<Location>();

  if (this->current_destination == Position::invalid ||
      Position::distance(this->current_destination, target_location->position()) > 100)
    {
      log_debug("Calculating path to target (because it moved, or to init)");
      this->current_destination = target_location->position();
      this->path = world->calculate_path(this->current_destination, this->location);
    }
  // Stop trying to move further, when we are at very close distance
  // (touching the followed entity).
  if (Position::distance(this->location->position(), this->current_destination) > 40)
    this->mobility->follow_path(this->path, world, this->location);
  return false;
}

