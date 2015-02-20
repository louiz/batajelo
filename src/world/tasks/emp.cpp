#include <world/tasks/emp.hpp>
#include <world/entity.hpp>
#include <world/world.hpp>
#include <world/world_callbacks.hpp>
#include <world/team.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/location.hpp>

#include <logging/logging.hpp>

static const Fix16 emp_radius = 300;

EmpTask::EmpTask(Entity* entity):
  Task(entity),
  duration(250)
{
}

bool EmpTask::tick(World* world)
{
  if (!--this->duration)
    {
      log_debug("emp explose");
      Team* own_team = this->entity->get<Team>();
      Location* own_location = this->entity->get<Location>();
      assert(own_team);
      assert(own_location);
      // Find all ennemy entities in the explosion radius
      for (const auto& entity: world->entities)
        {
          Team* team = entity->get<Team>();
          if (!team || team->get() == own_team->get())
            continue;
          Location* location = entity->get<Location>();
          if (!location ||
              (Position::distance(own_location->position(),
                                  location->position()) > emp_radius))
            continue;
          Health* health = entity->get<Health>();
          ManaPool* mana = entity->get<ManaPool>();
          if (!health || !mana)
            continue;
          health->add(-500);
          mana->add(-500);
        }
      this->entity->kill();
      return true;
    }
  return false;
}
