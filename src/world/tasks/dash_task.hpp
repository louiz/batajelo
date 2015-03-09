#ifndef DASH_TASK_HPP_INCLUDED
#define DASH_TASK_HPP_INCLUDED

#include <world/task.hpp>
#include <world/position.hpp>
#include <functional>

class Location;
class Mobility;

class DashTask: public Task
{
public:
  DashTask(Entity* entity, const Position& goal, Fix16 speed, std::function<void(Entity*)> impact_cb,
           std::function<void(void)> goal_cb);
  ~DashTask() = default;
  TaskType get_type() const
  {
    return TaskType::Dash;
  }
  bool tick(World* world) override final;

private:
  const Position goal;
  const Fix16 speed;
  /**
   * Function called for each encountered entity
   */
  std::function<void(Entity*)> impact_callback;
  /**
   * Function called when the goal is reached
   */
  std::function<void(void)> goal_callback;
  Location* location;
  Mobility* mobility;

  DashTask(const DashTask&) = delete;
  DashTask(DashTask&&) = delete;
  DashTask& operator=(const DashTask&) = delete;
  DashTask& operator=(DashTask&&) = delete;
};


#endif /* DASH_TASK_HPP_INCLUDED */
