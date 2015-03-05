#ifndef FOLLOW_TASK_HPP_INCLUDED
#define FOLLOW_TASK_HPP_INCLUDED

#include <world/task.hpp>
#include <world/path.hpp>
#include <world/position.hpp>

#include <memory>

class Entity;
class World;
class Mobility;
class Location;

class FollowTask: public Task
{
public:
  FollowTask(Entity* entity, const std::weak_ptr<Entity>& target);
  ~FollowTask() = default;

  bool tick(World* world) override final;
  TaskType get_type() const override final
    { return TaskType::Follow; }

private:
  /**
   * The entity we follow. If it goes out of sight or disappears, we have
   * finished the task.
   */
  std::weak_ptr<Entity> target;
  /**
   * The position the target had when we calculated the path for the last
   * time. Each tick we check if the current position is too far from this
   * one, if yes we recalculate the entire path.
   */
  Position current_destination;
  /**
   * The path to track to go near the target. This path is recalculated if
   * the target's position changed too much.
   */
  Path path;
  /**
   * The entity elements that we need to execute the work on.
   */
  Mobility* mobility;
  Location* location;

  FollowTask(const FollowTask&) = delete;
  FollowTask(FollowTask&&) = delete;
  FollowTask& operator=(const FollowTask&) = delete;
  FollowTask& operator=(FollowTask&&) = delete;
};


#endif /* FOLLOW_TASK_HPP_INCLUDED */
