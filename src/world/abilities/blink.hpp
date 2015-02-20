#ifndef BLINK_HPP_INCLUDED
#define BLINK_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>

class Blink: public ActiveAbility<Blink>
{
public:
  Blink();
  ~Blink() = default;
  void cast(Entity* entity, World*, const Position& pos, const bool queue) override final;

private:
  Blink(const Blink&) = delete;
  Blink(Blink&&) = delete;
  Blink& operator=(const Blink&) = delete;
  Blink& operator=(Blink&&) = delete;
};

#endif /* BLINK_HPP_INCLUDED */
