#ifndef DASH_HPP_INCLUDED
#define DASH_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>

class Dash: public ActiveAbility<Dash>
{
public:
  Dash();
  ~Dash() = default;
  void cast(Entity* entity, World*, const Position& pos, const bool queue) override final;

private:
  Dash(const Dash&) = delete;
  Dash(Dash&&) = delete;
  Dash& operator=(const Dash&) = delete;
  Dash& operator=(Dash&&) = delete;
};

#endif /* DASH_HPP_INCLUDED */
