#ifndef ABILITY_WORK_HPP_INCLUDED
#define ABILITY_WORK_HPP_INCLUDED

#include <world/work.hpp>

#include <cstddef>

/**
 * Base class for any Work that consists of casting an ability.  It can only
 * be added on an entity with a Abilities component. It saves various things
 * like the Entity’s bs and fs cast durations.
 */

class Entity;

class AbilityWork: public Work
{
public:
  AbilityWork(Entity* entity);
  ~AbilityWork() = default;

protected:
  bool cast_point_reached;
  std::size_t frontswing;
  std::size_t backswing;

private:
  AbilityWork(const AbilityWork&) = delete;
  AbilityWork(AbilityWork&&) = delete;
  AbilityWork& operator=(const AbilityWork&) = delete;
  AbilityWork& operator=(AbilityWork&&) = delete;
};


#endif /* ABILITY_WORK_HPP_INCLUDED */
