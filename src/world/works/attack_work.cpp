#include <world/works/attack_work.hpp>
#include <world/entity.hpp>
#include <world/position.hpp>
#include <world/world.hpp>
#include <world/health.hpp>
#include <world/team.hpp>
#include <world/tasks/attack_task.hpp>
#include <world/tasks/path_task.hpp>

#include <logging/logging.hpp>

AttackWork::AttackWork(Entity* entity, const Position& destination):
  Work(entity),
  location(entity->get<Location>()),
  acquire_distance(380_fix),
  destination(destination)
{
  assert(location);
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
      Fix16 distance = Position::distance(location->position(),
                                          this->location->position());
      if (distance < this->acquire_distance &&
          (res == world->entities.end() || distance < best_distance))
        {
          best_distance = distance;
          res = it;
        }
    }
  if (res != world->entities.end())
    this->set_task(world, std::make_unique<AttackTask>(this->entity, *res));
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

  // If we are not attacking, try to acquire a target.  If we find one, we
  // set our task to an AttackTask
  this->try_acquire_target(world);
  if (!this->task)
    this->set_task(world, std::make_unique<PathTask>(this->entity, this->destination));
  bool res = this->task->tick(world);
  if (this->task->get_type() == TaskType::Path)
    return res;
  return false;
}
