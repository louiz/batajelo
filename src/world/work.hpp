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
};

class PathWork: public Work
{
  friend class Unit;
public:
  PathWork(Unit*, const Position&);
  ~PathWork();
  virtual bool tick(World* world) override final;
private:
  PathWork(const PathWork&);
  PathWork& operator=(const PathWork&);
  /**
   * The path to follow. If it’s empty and the calculated bool is at false,
   * we calculate the path.  If it’s not empty, the unit needs to follow it
   * at each tick() If it’s empty and calculated is at true, destination has
   * been reached and the work is now done.
   */
  Path path;
  /**
   * We use that position only to calculate the path. Once it is calculated
   * we do not need it anymore.
   */
  const Position destination;
  /**
   * The unit to act upon
   */
  Unit* const unit;
  /**
   * Just a bool to know if the empty path means that we are at the end of
   * the path (thus return true directly) or that we didn’t calculate the
   * path yet.
   */
  bool calculated;
};

#endif // __WORK_HPP__
