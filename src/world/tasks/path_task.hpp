#ifndef PATH_TASK_HPP_INCLUDED
#define PATH_TASK_HPP_INCLUDED

#include <world/task.hpp>

#include <world/position.hpp>
#include <world/path.hpp>

class Mobility;
class Location;
class Entity;

class PathTask: public Task
{
public:
  PathTask(Entity* entity, const Position& destination);
  ~PathTask() = default;
  bool tick(World* world) override final;
  TaskType get_type() const override final
  { return TaskType::Path; }

private:
  /**
   * The path to follow. If it’s empty and the calculated bool is at false,
   * we calculate the path.  If it’s not empty, the unit needs to follow it
   * at each tick() If it’s empty and calculated is at true, destination has
   * been reached and the work is now done.
   */
  Path path;
  /**
   * We use that position only to calculate the path. Once it is calculated
   * we do not need it anymore.
   */
  const Position destination;
  /**
   * The entity elements that we need to execute the work on.
   */
  Mobility* mobility;
  Location* location;
  /**
   * Just a bool to know if the empty path means that we are at the end of
   * the path (thus return true directly) or that we didn’t calculate the
   * path yet.
   */
  bool calculated;

  PathTask(const PathTask&) = delete;
  PathTask(PathTask&&) = delete;
  PathTask& operator=(const PathTask&) = delete;
  PathTask& operator=(PathTask&&) = delete;
};


#endif /* PATH_TASK_HPP_INCLUDED */
