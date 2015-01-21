#ifndef __WORK_HPP__
# define __WORK_HPP__

class World;

/**
 * The function to call at each tick of the entity owning that work. It
 * returns true if that work is complete and must be removed from the queue,
 * false otherwise.
 */

class Work
{
public:
  Work() = default;
  virtual ~Work() {}
  virtual bool tick(World* world) = 0;

private:
  Work(const Work&);
  Work& operator=(const Work&);
  Work(Work&&);
  Work& operator=(Work&&);
};

#endif // __WORK_HPP__
