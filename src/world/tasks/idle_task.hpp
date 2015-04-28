#ifndef IDLE_TASK_HPP_INCLUDED
#define IDLE_TASK_HPP_INCLUDED

#include <world/task.hpp>

class IdleTask: public Task
{
public:
  static IdleTask that;

  ~IdleTask() = default;
  TaskType get_type() const override final
  {
    return TaskType::None;
  }
  bool tick(World*) override final { return true; }

private:
  IdleTask():
    Task(nullptr) {}
  IdleTask(const IdleTask&) = delete;
  IdleTask(IdleTask&&) = delete;
  IdleTask& operator=(const IdleTask&) = delete;
  IdleTask& operator=(IdleTask&&) = delete;
};

#endif /* IDLE_TASK_HPP_INCLUDED */

