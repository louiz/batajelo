/**
 *
 */

#ifndef __WORK_HPP__
# define __WORK_HPP__

#include <boost/function.hpp>
#include <boost/bind.hpp>

class Unit;
class Building;
class Entity;

#include <world/position.hpp>

/**
 * The function to call at each tick of the entity owning that work. It returns true if that work is complete and must be removed from the queue, false otherwise.
 */
typedef boost::function<bool (World*, Work*)> t_work_callback;

class Work
{
public:
  Work(t_work_callback c):
    work_callback(c) {}
  bool operator()(World* world, Work* work)
  {
    return this->work_callback(world, work);
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
  PathWork(Unit*, Position);
  ~PathWork();
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
  Position end_position;
  /**
   * Just a bool to know if the empty path means that we are at the end of the path (thus return true directly) or that we didn’t calculate the path yet.
   */
  bool calculated;
};

#endif // __WORK_HPP__
