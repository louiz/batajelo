#ifndef ACTIVE_ABILITY_HPP_INCLUDED
#define ACTIVE_ABILITY_HPP_INCLUDED

#include <string>

#include <world/abilities/named_ability.hpp>

enum class TargetType
{
  Point,    // Target a specific world position, or an area (centered around
            // the point)
  Entity,   // Target an entity
  None,     // No target needs to be given (a global or a targetting the
            // casting entity itself)
};

template<typename DerrivedClass>
class ActiveAbility: public NamedAbility<DerrivedClass>
{
public:
  ActiveAbility(const TargetType target_type):
    target_type(target_type)
    {}
  ~ActiveAbility() = default;
  void tick(Entity*, World*) override final {}

private:
  TargetType target_type;
  ActiveAbility(const ActiveAbility&) = delete;
  ActiveAbility(ActiveAbility&&) = delete;
  ActiveAbility& operator=(const ActiveAbility&) = delete;
  ActiveAbility& operator=(ActiveAbility&&) = delete;
};

#endif /* ACTIVE_ABILITY_HPP_INCLUDED */
