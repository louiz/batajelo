#include <world/tasks/dash_task.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/world.hpp>

DashTask::DashTask(Entity* entity, const Position& goal, Fix16 speed,
                   Fix16 max_distance, Fix16 impact_width, std::function<void(Entity*)> impact_cb,
                   std::function<void(void)> goal_cb):
  Task(entity),
  goal(goal),
  speed(speed),
  max_distance(max_distance),
  impact_width(impact_width),
  travelled_distance(0),
  impact_callback(impact_cb),
  goal_callback(goal_cb),
  impacted_entities{},
  mobility(entity->get<Mobility>()),
  location(entity->get<Location>())
{
  assert(this->mobility);
  assert(this->location);
}

bool DashTask::tick(World* world)
{
  this->travelled_distance += this->mobility->move_towards(this->goal, this->location, this->speed);

  if (this->impact_callback)
    {
      // Check for an impact with any entity
      for (const std::shared_ptr<Entity>& entity: world->entities)
        {
          Location* entity_location = entity->get<Location>();
          if (!entity_location)
            continue;
          if (std::find(this->impacted_entities.begin(), this->impacted_entities.end(),
                        entity.get()) != this->impacted_entities.end())
            continue;
          if (Position::distance(entity_location->position(), this->location->position()) <= this->impact_width)
            {
              this->impact_callback(entity.get());
              this->impacted_entities.push_back(entity.get());
            }
        }
    }
  if (this->goal == this->location->position() || this->travelled_distance >= this->max_distance)
    {
      // TODO adjust location to be on a valid world position
      if (this->goal_callback)
        this->goal_callback();
      return true;
    }
  return false;
}
