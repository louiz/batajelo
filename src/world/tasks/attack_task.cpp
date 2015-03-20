#include <world/tasks/attack_task.hpp>

#include <world/world.hpp>
#include <world/world_callbacks.hpp>
#include <world/entity.hpp>
#include <world/location.hpp>
#include <world/health.hpp>
#include <world/works/projectile_work.hpp>

AttackTask::AttackTask(Entity* entity, std::weak_ptr<Entity> target, const std::size_t fs, const std::size_t bs, const bool ranged):
  Task(entity),
  target(target),
  location(entity->get<Location>()),
  frontswing(fs),
  backswing(bs),
  attack_point_reached(false),
  ranged(ranged)
{
  assert(location);
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
      world->callbacks->ability_casted(this->entity, AbilityType::Attack,
                                       target.get(), Position::zero);
      this->do_attack(world);
    }
  if (!this->backswing)
    return true;
  this->backswing--;
  return false;
}

void AttackTask::do_attack(World* world)
{
  auto cb = [](Entity* target) -> void
    {
      Health* health = target->get<Health>();
      assert(health);
      health->add(-10);
    };
  if (this->ranged)
    {
      Entity* projectile = world->do_new_entity(1, this->location->position(), 1);
      projectile->set_work(
                           std::make_unique<ProjectileWork>(projectile,
                                                            this->target,
                                                            std::move(cb)));
    }
  else
    cb(this->target.lock().get());
}
