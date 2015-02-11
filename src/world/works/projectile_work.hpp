#ifndef PROJECTILE_WORK_HPP_INCLUDED
#define PROJECTILE_WORK_HPP_INCLUDED

#include <memory>

#include <world/work.hpp>

/**
 * Simple Work where the entity just follows (without path finding or
 * collision or anything) an other entity, until the entity is reach (in
 * wich case an event occurs, for example explosion, or damages, etc) or the
 * entity disapears (in that case we now follow the last known position of
 * the entity, and when it’s reach we do an other event, most of the time
 * nothing)
 */

class ProjectileWork: public Work
{
public:
  ProjectileWork(Entity* entity, std::weak_ptr<Entity> target,
                 const int dmg);
  ~ProjectileWork() = default;
  bool tick(World* world) override final;

private:
  /**
   * Info about our goal
   */
  std::weak_ptr<Entity> target;
  /**
   * Info about the event when we reach our goal.
   */
  const int dmg;

  ProjectileWork(const ProjectileWork&) = delete;
  ProjectileWork(ProjectileWork&&) = delete;
  ProjectileWork& operator=(const ProjectileWork&) = delete;
  ProjectileWork& operator=(ProjectileWork&&) = delete;
};

#endif /* PROJECTILE_WORK_HPP_INCLUDED */
