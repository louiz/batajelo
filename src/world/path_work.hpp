#ifndef PATH_WORK_HPP_INCLUDED
#define PATH_WORK_HPP_INCLUDED

#include <world/work.hpp>
#include <world/position.hpp>
#include <world/path.hpp>

class Entity;
class Mobility;
class Location;

class PathWork: public Work
{
public:
  PathWork(Entity* entity, const Position& destination);
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
   * The entity elemets that we need to execute the work.
   */
  Mobility* mobility;
  Location* location;
  /**
   * Just a bool to know if the empty path means that we are at the end of
   * the path (thus return true directly) or that we didn’t calculate the
   * path yet.
   */
  bool calculated;
};

#endif /* PATH_WORK_HPP_INCLUDED */

