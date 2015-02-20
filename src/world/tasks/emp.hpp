#ifndef EMP_TASK_HPP_INCLUDED
#define EMP_TASK_HPP_INCLUDED

#include <world/task.hpp>

class EmpTask: public Task
{
public:
  EmpTask(Entity* entity);
  ~EmpTask() = default;
  bool tick(World* world) override final;
  TaskType get_type() const
  {
    return TaskType::Emp;
  }

private:
  /**
   * Explosion duration, in ticks
   */
  unsigned int duration;
  EmpTask(const EmpTask&) = delete;
  EmpTask(EmpTask&&) = delete;
  EmpTask& operator=(const EmpTask&) = delete;
  EmpTask& operator=(EmpTask&&) = delete;
};

#endif /* EMP_TASK_HPP_INCLUDED */
