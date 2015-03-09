#ifndef CONCENTRATE_TASK_HPP_INCLUDED
#define CONCENTRATE_TASK_HPP_INCLUDED

#include <world/task.hpp>
#include <fixmath/fix16.hpp>

#include <functional>

class ConcentrateTask: public Task
{
public:
  ConcentrateTask(Entity* entity);
  ~ConcentrateTask() = default;
  bool tick(World* world) override final;
  TaskType get_type() const override final
  {
    return TaskType::Concentrate;
  }
  Fix16 value() const;

private:
  Fix16 quantity;

  ConcentrateTask(const ConcentrateTask&) = delete;
  ConcentrateTask(ConcentrateTask&&) = delete;
  ConcentrateTask& operator=(const ConcentrateTask&) = delete;
  ConcentrateTask& operator=(ConcentrateTask&&) = delete;
};


#endif /* CONCENTRATE_TASK_HPP_INCLUDED */
