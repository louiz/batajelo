#ifndef PROJECTILE_TASK_HPP_INCLUDED
#define PROJECTILE_TASK_HPP_INCLUDED

#include <world/task.hpp>
#include <world/position.hpp>
#include <memory>

class Location;
class Mobility;

/**
 * Very simple Task that just follows a target, or the last target position
 * if the target disappears.
 */

class ProjectileTask: public Task
{
public:
  ProjectileTask(Entity* entity, std::weak_ptr<Entity> target);
  ~ProjectileTask() = default;
  TaskType get_type() const
  {
    return TaskType::Projectile;
  }
  bool tick(World* world) override final;

private:
  /**
   * Our own informations
   */
  Location* location;
  Mobility* mobility;
  /**
   * Info about our goal
   */
  std::weak_ptr<Entity> target;
  Position last_known_position;

  ProjectileTask(const ProjectileTask&) = delete;
  ProjectileTask(ProjectileTask&&) = delete;
  ProjectileTask& operator=(const ProjectileTask&) = delete;
  ProjectileTask& operator=(ProjectileTask&&) = delete;
};


#endif /* PROJECTILE_TASK_HPP_INCLUDED */
