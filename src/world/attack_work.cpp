#include <world/attack_work.hpp>
#include <world/entity.hpp>
#include <world/position.hpp>
#include <world/world.hpp>
#include <world/health.hpp>

#include <logging/logging.hpp>

AttackWork::AttackWork(Entity* entity, const Position& position):
  Work(entity),
  path_work(entity, position),
  location(entity->get<Location>()),
  entity(entity),
  acquire_distance(200_fix),
  attack_point_reached(false)
{
  assert(location);
  this->reset();
}

bool AttackWork::try_acquire_target(World* world)
{
  if (world->entities.size() <= 1)
    return false; // If we are the only entity in the world
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
    {
      this->locked_target = *res;
      return true;
    }
  return false;
}

void AttackWork::attack_steps(World* world)
{
  if (this->frontswing)
    {
      this->frontswing--;
      return;
    }
  if (!this->attack_point_reached)
    {
      auto target = this->locked_target.lock();
      log_debug("Attacking now " << target->get_id());
      Health* health = target->get<Health>();
      health->add(-15);
      this->attack_point_reached = true;
    }
  if (this->backswing)
    {
      this->backswing--;
      if (!this->backswing)
        {
          this->reset();
        }
    }
}

void AttackWork::reset()
{
  this->frontswing = 30;
  this->backswing = 30;
  this->attack_point_reached = false;
  this->locked_target.reset();
}

bool AttackWork::tick(World* world)
{
  if (locked_target.expired())
    {
      this->try_acquire_target(world);
    }
  if (locked_target.expired())
    { // Move towards the goal
      if (this->path_work.tick(world) == true)
        return true;
    }
  else
    { // attack the locked entity
      this->attack_steps(world);
    }
  return false;
}
