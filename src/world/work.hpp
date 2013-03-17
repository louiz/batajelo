/**
 *
 */

#ifndef __WORK_HPP__
# define __WORK_HPP__

#include <functional>

class Unit;
class Building;
class Entity;

#include <world/position.hpp>
#include <world/path.hpp>

/**
 * The function to call at each tick of the entity owning that work. It returns true if that work is complete and must be removed from the queue, false otherwise.
 */
typedef std::function<bool (World*, Work*)> t_work_callback;

class Work
{
public:
  Work(t_work_callback callback):
    work_callback(callback) {}
  virtual ~Work() {};
  bool operator()(World* world, Work* work)
  {
    return this->work_callback(world, work);
  }
  /**
   * Whether or not the work can be interrupted. A Work object that is not
   * interruptible means that it is not removed from the queue when we call
   * entity->set_work() and that this work at the front of the queue
   * (meaning that it's the current work). It is however removed from the
   * queue if it's not at the front.  Herited class just need to override
   * that method, and return false, if they are not interruptible.
   */
  inline virtual bool is_interruptible() const
  {
    return true;
  }
protected:
  t_work_callback work_callback;
private:
  Work(const Work&);
  Work& operator=(const Work&);
};

class PathWork: public Work
{
  friend class Unit;
public:
  PathWork(Unit*, const Position);
  ~PathWork() {}
private:
  PathWork(const PathWork&);
  PathWork& operator=(const PathWork&);
  /**
   * The path to follow. If it’s empty and the calculated bool is at false,
   * we calculate the path. Otherwise, the work is done.
   */
  Path path;
  /**
   * We use that position only to calculate the path. Once it is calculated
   * we do not need it anymore.
   */
  const Position end_position;
  /**
   * Just a bool to know if the empty path means that we are at the end of the path (thus return true directly) or that we didn’t calculate the path yet.
   */
  bool calculated;
};

class RallyWork: public Work
{
  friend class Building;
public:
  RallyWork(Building*, const Position);
  ~RallyWork() {}
private:
  const Position position;
};

class BuildWork: public Work
{
  friend class Unit;
public:
  BuildWork(Unit*, const unsigned short, const short, const short);
  ~BuildWork() {}
private:
  BuildWork(const BuildWork&);
  BuildWork& operator=(const BuildWork&);
  const unsigned short id;
  const short x;
  const short y;
  /**
   * A pointer to the building being built during that work. It's at 0 if
   * the build has not yet been started (so this is a new build, and the
   * work is yet still queued in the unit).
   */
  Building* building;
};

class SpawnWork: public Work
{
  friend class Building;
public:
  SpawnWork(Building*, const unsigned short);
  ~SpawnWork() {}
private:
  SpawnWork(const SpawnWork&);
  SpawnWork& operator=(const SpawnWork&);
  /**
   * The type_id of the unit to spawn.
   */
  const unsigned short type_id;
  /**
   * The elapsed time of the spawning of the unit. The work is done when
   * it reaches the unit->spawn_duration number.
   */
  unsigned int elapsed_time;
};

#endif // __WORK_HPP__
