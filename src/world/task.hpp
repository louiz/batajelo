#ifndef TASK_HPP_INCLUDED
#define TASK_HPP_INCLUDED

enum class TaskType
{
  None,
  Path,
  Follow,
  Attack,
  Blink,
  Emp,
  Projectile,

  count
};

class World;
class Entity;

class Task
{
public:
  Task(Entity* entity):
    entity(entity) {}
  virtual ~Task() = default;
  /**
   * Execute the actual task, until it returns true to indicate that it's
   * finished
   */
  virtual bool tick(World*) = 0;
  /**
   * Each Task has its own TaskType
   */
  virtual TaskType get_type() const = 0;
  /**
   * Returns whether or not, when this task is being destroyed due to the
   * parent Work being interrupted, we need to continue the work or not. If
   * we return false, the parent Work needs to go on its next task (or be
   * over, if it was the last one).  This is useful for example whenever an
   * AbilityTask already casted the actual ability, and is being interrupted
   * in its backswing period: since it has already been casted, the task is
   * over and the work should not recreate that AbilityTask.
   */
  virtual bool needs_to_be_restarted() const
  {
    return true;
  }

protected:
  Entity* entity;

private:
  Task(const Task&) = delete;
  Task(Task&&) = delete;
  Task& operator=(const Task&) = delete;
  Task& operator=(Task&&) = delete;
};

#endif /* TASK_HPP_INCLUDED */
