#ifndef ABILITY_TASK_HPP_INCLUDED
#define ABILITY_TASK_HPP_INCLUDED

#include <world/task.hpp>

#include <cstddef>

/**
 * Base class for any Task that consists of casting an ability.  It can only
 * be added on an entity with a Abilities component. It saves various things
 * like the Entity’s bs and fs cast durations.
 */

class Entity;

class AbilityTask: public Task
{
public:
  AbilityTask(Entity* entity);
  ~AbilityTask() = default;
  bool needs_to_be_restarted() const override final;

protected:
  bool cast_point_reached;
  std::size_t frontswing;
  std::size_t backswing;

private:
  AbilityTask(const AbilityTask&) = delete;
  AbilityTask(AbilityTask&&) = delete;
  AbilityTask& operator=(const AbilityTask&) = delete;
  AbilityTask& operator=(AbilityTask&&) = delete;
};


#endif /* ABILITY_TASK_HPP_INCLUDED */
