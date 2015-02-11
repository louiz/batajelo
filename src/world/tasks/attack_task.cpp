#include <world/tasks/attack_task.hpp>

#include <world/world.hpp>
#include <world/world_callbacks.hpp>
#include <world/location.hpp>
#include <world/entity.hpp>
#include <world/works/projectile_work.hpp>

AttackTask::AttackTask(Entity* entity, std::weak_ptr<Entity> target):
  Task(entity),
  target(target),
  location(entity->get<Location>()),
  frontswing(30u),
  backswing(30u),
  attack_point_reached(false)
{
  assert(location);
  // Set front and backswing from the entity
}

bool AttackTask::tick(World* world)
{
  if (this->target.expired())
    return true;

  if (this->frontswing)
    {
      this->frontswing--;
      return false;
    }

  // Frontswing step is finished
  if (!this->attack_point_reached)
    {
      // Do the actual attack
      auto target = this->target.lock();
      this->attack_point_reached = true;
      if (world->callbacks->ability_casted)
        world->callbacks->ability_casted(this->entity, AbilityType::Attack,
                                         target.get(), Position::zero);
      // Only if ranged entity
      Entity* projectile = world->do_new_entity(1, this->location->position(), 1);
      projectile->set_work(
          std::make_unique<ProjectileWork>(projectile, this->target, 7u));
    }
  if (this->backswing)
    {
      this->backswing--;
      return false;
    }
  else
    return true;
}
