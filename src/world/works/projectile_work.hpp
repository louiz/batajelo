#ifndef PROJECTILE_WORK_HPP_INCLUDED
#define PROJECTILE_WORK_HPP_INCLUDED

#include <world/work.hpp>

#include <functional>
#include <memory>

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
  ProjectileWork(World* world, Entity* entity, std::weak_ptr<Entity> target,
                 std::function<void(Entity*)> callback);
  ~ProjectileWork() = default;
  bool tick() override final;

private:
  /**
   * Info about our goal
   */
  std::weak_ptr<Entity> target;
  /**
   * Called when the entity hits the target, if there is one
   */
  std::function<void(Entity*)> callback;

  ProjectileWork(const ProjectileWork&) = delete;
  ProjectileWork(ProjectileWork&&) = delete;
  ProjectileWork& operator=(const ProjectileWork&) = delete;
  ProjectileWork& operator=(ProjectileWork&&) = delete;
};

#endif /* PROJECTILE_WORK_HPP_INCLUDED */
