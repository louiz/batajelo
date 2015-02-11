#ifndef ATTACK_TASK_HPP_INCLUDED
#define ATTACK_TASK_HPP_INCLUDED

#include <world/task.hpp>

#include <memory>

class Location;

class AttackTask: public Task
{
public:
  AttackTask(Entity* entity, std::weak_ptr<Entity> target);
  ~AttackTask() = default;
  bool tick(World* world) override final;
  TaskType get_type() const override final
  { return TaskType::Attack; }

private:
  /**
   * If the target becomes unavailable before the attack point is reached,
   * this task is over.
   */
  std::weak_ptr<Entity> target;
  Location* location;

  std::size_t frontswing;
  std::size_t backswing;
  bool attack_point_reached;

  AttackTask(const AttackTask&) = delete;
  AttackTask(AttackTask&&) = delete;
  AttackTask& operator=(const AttackTask&) = delete;
  AttackTask& operator=(AttackTask&&) = delete;
};


#endif /* ATTACK_TASK_HPP_INCLUDED */
