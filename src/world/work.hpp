#ifndef __WORK_HPP__
# define __WORK_HPP__

#include <world/task.hpp>

#include <memory>

class World;
class Entity;

/**
 * A work is composed of one or more Task. Each Work::tick(), we call
 * task.tick(). We may also change the value of the current work, depending
 * on some world condition, or if the current task finished (tick() returned
 * true)
 */

class Work
{
public:
  Work(World* world, Entity* entity):
    entity(entity),
    world(world) {}
  virtual ~Work();
  /**
   * The function to call at each tick of the entity owning that work. It
   * returns true if that work is complete and must be removed from the queue,
   * false otherwise.
   *
   * May set/change the current task. Then call tick() on that Task.
   */
  virtual bool tick() = 0;
  /**
   * Cancel the current Task. The work is not however canceled, just
   * interrupted.  The task may be recreated on the next call to tick.
   * Basically, a mini-stun will interrupt the current work and the work
   * will be restarted on the next tick.  For example if an AttackTask was
   * ongoing, it will be deleted and recreated immediately on the next
   * tick() call (thus resetting the animation)
   *
   * Calls the on_interrupted method of the subclass
   */
  void interrupt();
  Task* get_task();
  const Task* get_task() const;

  template <typename TaskType>
  TaskType* get_task()
  {
    return dynamic_cast<TaskType*>(this->get_task());
  }
  template <typename TaskType>
  const TaskType* get_task() const
  {
    return dynamic_cast<const TaskType*>(this->get_task());
  }

protected:
  /**
   * Set the current Task of this work. Also call
   * WorldCallbacks::task_changed() with the correct parameters
   */
  void set_task(std::unique_ptr<Task> task);
  /**
   * Keep a pointer to the entity, so we can know which one it is. Pass that
   * pointer when creating a Task, which will in turn get() the needed
   * component for this Task.
   */
  Entity* entity;
  World* world;
  /**
   * The current task being executed. May be nullptr at any moment.
   */
  std::unique_ptr<Task> task;

private:
  virtual void on_interrupted() {}

  Work(const Work&) = delete;
  Work& operator=(const Work&) = delete;
  Work(Work&&) = delete;
  Work& operator=(Work&&) = delete;
};

#endif // __WORK_HPP__
