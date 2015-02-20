#ifndef SINGLE_TASK_WORK_HPP_INCLUDED
#define SINGLE_TASK_WORK_HPP_INCLUDED

#include <world/work.hpp>

template <typename TaskClass>
class SingleTaskWork: public Work
{
public:
  template<typename... ArgsType>
  SingleTaskWork(Entity* entity, World* world, ArgsType&&... args):
    Work(entity)
  {
    this->set_task(world,
                   std::make_unique<TaskClass>(entity,
                                               std::forward<ArgsType>(args)...));
  }
  ~SingleTaskWork() = default;
  bool tick(World* world) override final
  {
    if (!this->task)
      {
        // We were interrupted, this kind of work is not resumable, so
        // consider us finished
        return true;
      }
    return this->task->tick(world);
  }

private:
  SingleTaskWork(const SingleTaskWork&) = delete;
  SingleTaskWork(SingleTaskWork&&) = delete;
  SingleTaskWork& operator=(const SingleTaskWork&) = delete;
  SingleTaskWork& operator=(SingleTaskWork&&) = delete;
};

#endif /* SINGLE_TASK_WORK_HPP_INCLUDED */
