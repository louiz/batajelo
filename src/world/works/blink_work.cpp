#include <world/works/blink_work.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/position.hpp>
#include <world/world.hpp>
#include <world/world_callbacks.hpp>

#include <world/tasks/blink_task.hpp>
#include <world/tasks/path_task.hpp>

#include <logging/logging.hpp>

#include <cassert>

const Fix16 BlinkWork::cast_distance = 600;

BlinkWork::BlinkWork(World* world, Entity* entity, const Position& destination):
  Work(world, entity),
  destination(destination),
  location(entity->get<Location>())
{
}

bool BlinkWork::tick()
{
  if (Position::distance(this->location->position(), this->destination) > this->cast_distance)
    {
      if (!this->task)
        this->set_task(std::make_unique<PathTask>(this->entity, this->destination));
    }
  else
    {
      if (!this->task || this->task->get_type() != TaskType::Blink)
        this->set_task(std::make_unique<BlinkTask>(this->entity, this->destination));
    }
  bool result = this->task->tick(this->world);
  // We only finish this work if the BlinkTask finished
  if (this->task->get_type() == TaskType::Blink)
    return result;
  return false;
}
