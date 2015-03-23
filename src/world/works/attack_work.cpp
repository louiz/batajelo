#include <world/works/attack_work.hpp>
#include <world/entity.hpp>
#include <world/position.hpp>
#include <world/world.hpp>
#include <world/health.hpp>
#include <world/location.hpp>
#include <world/team.hpp>
#include <world/tasks/attack_task.hpp>
#include <world/tasks/path_task.hpp>
#include <world/tasks/follow_task.hpp>
#include <world/abilities.hpp>
#include <world/abilities/attack.hpp>

#include <logging/logging.hpp>

AttackWork::AttackWork(Entity* entity, const Position& destination, const Fix16 range):
  AttackWork(entity, {}, range, destination)
{
}

AttackWork::AttackWork(Entity* entity, std::weak_ptr<Entity> target, const Fix16 range,
                       const Position& destination):
  Work(entity),
  location(entity->get<Location>()),
  destination(destination),
  target(target),
  attack(static_cast<Attack*>(entity->get<Abilities>()->find(AbilityType::Attack))),
  range(range)
{
  assert(this->location);
  assert(this->attack);
}

void AttackWork::try_acquire_target(World* world)
{
  if (world->entities.size() <= 1)
    return ; // If we are the only entity in the world
  Fix16 best_distance;
  auto res = world->entities.end();
  for (auto it = world->entities.begin();
       it != world->entities.end(); ++it)
    {
      if ((*it).get() == this->entity)
        continue;
      Location* location = (*it)->get<Location>();
      if (!location)
        continue;
      Team* team = (*it)->get<Team>();
      if (!team || team->get() == this->entity->get<Team>()->get())
        continue;
      Health* health = (*it)->get<Health>();
      if (!health)
        continue;
      Fix16 distance =
          Position::distance(location->position(), this->location->position()) -
          this->location->get_width() - location->get_width() - 20;
      log_debug("try_acquire_target: " << distance << "(" << this->location->get_width() << "," << location->get_width() << " < " << this->range << "?");
      if (distance <= this->range &&
          (res == world->entities.end() || distance < best_distance))
        {
          best_distance = distance;
          res = it;
        }
    }
  if (res != world->entities.end())
    this->set_task(world,
                   std::make_unique<AttackTask>(this->entity, *res,
                                                      this->attack->get_frontswing_duration(),
                                                      this->attack->get_backswing_duration(),
                                                      this->range > 0_fix));
}

bool AttackWork::tick(World* world)
{
  // If we are attacking, execute the task until it's over
  if (this->task && this->task->get_type() == TaskType::Attack)
    {
      if (this->task->tick(world) == true)
        this->task.reset(nullptr);
      return false;
    }

  // If we are attacking a position
  if (this->destination != Position::invalid)
    {
      // Try to acquire a target.  If we find one, we
      // set our task to an AttackTask
      this->try_acquire_target(world);
      // If we did not find any, just follow the path to destination
      if (!this->task)
        this->set_task(world, std::make_unique<PathTask>(this->entity,
                                                         this->destination));
    }
  else
    {
      // If we are follow-attacking a specific target, follow it until it is
      // within reach.

      // Find out if the target is in our reach
      Location* target_location = this->target.lock()->get<Location>();
      assert(target_location);
      Fix16 distance =
          Position::distance(target_location->position(), this->location->position()) -
          this->location->get_width() - location->get_width() - 20;
      if (distance < this->range)
        this->set_task(world, std::make_unique<AttackTask>(this->entity, this->target,
                                                           this->attack->get_frontswing_duration(),
                                                           this->attack->get_backswing_duration(),
                                                           this->range > 0_fix));
      else if (!this->task)
        this->set_task(world, std::make_unique<FollowTask>(this->entity, this->target));
    }

  bool res = this->task->tick(world);
  if (this->task->get_type() == TaskType::Path ||
      this->task->get_type() == TaskType::Follow)
    return res;
  return false;
}
