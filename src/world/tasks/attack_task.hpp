#ifndef ATTACK_TASK_HPP_INCLUDED
#define ATTACK_TASK_HPP_INCLUDED

#include <world/task.hpp>

#include <memory>

class Location;

class AttackTask: public Task
{
public:
  AttackTask(Entity* entity, std::weak_ptr<Entity> target, const std::size_t fs,
             const std::size_t bs, const bool ranged);
  ~AttackTask() = default;
  bool tick(World* world) override final;
  TaskType get_type() const override final
  { return TaskType::Attack; }
  /**
   * in ticks
   */
  std::size_t get_remaining_frontswing_duration() const;
  std::size_t get_remaining_backswing_duration() const;

private:
  void do_attack(World* world);
  /**
   * If the target becomes unavailable before the attack point is reached,
   * this task is over.
   */
  std::weak_ptr<Entity> target;
  Location* location;
  /**
   * Number of ticks needed to complete the front and backswings
   */
  std::size_t frontswing;
  std::size_t backswing;
  bool attack_point_reached;
  const bool ranged;

  AttackTask(const AttackTask&) = delete;
  AttackTask(AttackTask&&) = delete;
  AttackTask& operator=(const AttackTask&) = delete;
  AttackTask& operator=(AttackTask&&) = delete;
};


#endif /* ATTACK_TASK_HPP_INCLUDED */
