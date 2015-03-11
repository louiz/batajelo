#include <world/works/projectile_work.hpp>
#include <world/tasks/projectile_task.hpp>
#include <world/entity.hpp>
#include <world/world.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/health.hpp>
#include <world/world_callbacks.hpp>

ProjectileWork::ProjectileWork(Entity* entity, std::weak_ptr<Entity> target,
                               std::function<void(Entity*)> callback):
  Work(entity),
  target(target),
  callback(callback)
{
}

bool ProjectileWork::tick(World* world)
{
  if (!this->task)
    this->set_task(world, std::make_unique<ProjectileTask>(this->entity, this->target));
  if (this->task->tick(world))
    {
      // Position or target has been reached
      this->entity->kill();
      if (!this->target.expired())
        {
          // impact
          Entity* targetp = this->target.lock().get();
          if (this->callback)
            this->callback(targetp);
          world->callbacks->impact(this->entity, targetp);
        }
      return true;
    }
  return false;
}
