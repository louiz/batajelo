#ifndef PHASE_HPP_INCLUDED
#define PHASE_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>

class Phase: public ActiveAbility<Phase>
{
public:
  Phase();
  ~Phase() = default;
  void cast(Entity* entity, World *, const Position& pos, const bool queue) override final;

private:
  Phase(const Phase&) = delete;
  Phase(Phase&&) = delete;
  Phase& operator=(const Phase&) = delete;
  Phase& operator=(Phase&&) = delete;
};


#endif /* PHASE_HPP_INCLUDED */
