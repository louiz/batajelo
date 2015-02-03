#ifndef __WORK_HPP__
# define __WORK_HPP__

class World;
class Entity;

/**
 * The function to call at each tick of the entity owning that work. It
 * returns true if that work is complete and must be removed from the queue,
 * false otherwise.
 */

class Work
{
public:
  Work(Entity* entity):
    entity(entity) {}
  virtual ~Work() {}
  virtual bool tick(World* world) = 0;

protected:
  /**
   * Keep a pointer to the entity, so we can know which one it is. But do
   * not use that pointer to do the actual work, get the individual
   * components pointers in the subclasses.
   */
  const Entity* entity;

private:
  Work(const Work&) = delete;
  Work& operator=(const Work&) = delete;
  Work(Work&&) = delete;
  Work& operator=(Work&&) = delete;
};

#endif // __WORK_HPP__
