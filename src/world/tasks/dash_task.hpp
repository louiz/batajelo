#ifndef DASH_TASK_HPP_INCLUDED
#define DASH_TASK_HPP_INCLUDED

#include <world/task.hpp>
#include <world/position.hpp>
#include <functional>
#include <vector>

class Location;
class Mobility;

class DashTask: public Task
{
public:
  DashTask(Entity* entity, const Position& goal, Fix16 speed,
           Fix16 max_distance, Fix16 impact_width, std::function<void(Entity*)> impact_cb,
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
  const Fix16 max_distance;
  const Fix16 impact_width;
  Fix16 travelled_distance;
  /**
   * Function called for each encountered entity
   */
  std::function<void(Entity*)> impact_callback;
  /**
   * Function called when the goal is reached
   */
  std::function<void(void)> goal_callback;
  /**
   * A list of already impacted entities, to avoid calling the callback many
   * times on the same one
   */
  std::vector<Entity*> impacted_entities;

  Location* location;
  Mobility* mobility;

  DashTask(const DashTask&) = delete;
  DashTask(DashTask&&) = delete;
  DashTask& operator=(const DashTask&) = delete;
  DashTask& operator=(DashTask&&) = delete;
};


#endif /* DASH_TASK_HPP_INCLUDED */
