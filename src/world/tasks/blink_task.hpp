#ifndef BLINK_TASK_HPP_INCLUDED
#define BLINK_TASK_HPP_INCLUDED

#include <world/tasks/ability_task.hpp>
#include <world/position.hpp>

class Mobility;
class Location;

class BlinkTask: public AbilityTask
{
public:
  BlinkTask(Entity* entity, const Position& destination);
  ~BlinkTask() = default;
  bool tick(World* world) override final;
  TaskType get_type() const
  {
    return TaskType::Blink;
  }

private:
  /**
   * Our blink destination
   */
  const Position destination;
  /**
   * The entity elements that we need to execute the work on.
   */
  Mobility* mobility;
  Location* location;

  BlinkTask(const BlinkTask&) = delete;
  BlinkTask(BlinkTask&&) = delete;
  BlinkTask& operator=(const BlinkTask&) = delete;
  BlinkTask& operator=(BlinkTask&&) = delete;
};


#endif /* BLINK_TASK_HPP_INCLUDED */
