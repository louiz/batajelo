#ifndef PATH_WORK_HPP_INCLUDED
#define PATH_WORK_HPP_INCLUDED

#include <world/work.hpp>
#include <world/position.hpp>

class Entity;

class PathWork: public Work
{
public:
  PathWork(Entity* entity, const Position& destination);
  ~PathWork() = default;
  bool tick(World* world) override final;

private:
  const Position destination;

  PathWork(const PathWork&);
  PathWork& operator=(const PathWork&);
};

#endif /* PATH_WORK_HPP_INCLUDED */

