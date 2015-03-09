#ifndef CONCENTRATE_HPP_INCLUDED
#define CONCENTRATE_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>

/**
 * A very simple ability that just starts a ConcentrateWork until the entity
 * casts an other ability.  That other ability will probably use the value
 * of the ConcentrateWork, for example to determine its strength.
 */

class Concentrate: public ActiveAbility<Concentrate>
{
public:
  Concentrate();
  ~Concentrate() = default;
  void cast(Entity*, World*, const bool queue) override final;

private:
  Concentrate(const Concentrate&) = delete;
  Concentrate(Concentrate&&) = delete;
  Concentrate& operator=(const Concentrate&) = delete;
  Concentrate& operator=(Concentrate&&) = delete;
};


#endif /* CONCENTRATE_HPP_INCLUDED */
