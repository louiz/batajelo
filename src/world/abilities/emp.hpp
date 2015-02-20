#ifndef EMP_HPP_INCLUDED
#define EMP_HPP_INCLUDED

#include <world/abilities/active_ability.hpp>

class Emp: public ActiveAbility<Emp>
{
public:
  Emp();
  ~Emp() = default;
  void cast(Entity* entity, World*, const Position& pos,
            const bool queue) override final;

private:
  Emp(const Emp&) = delete;
  Emp(Emp&&) = delete;
  Emp& operator=(const Emp&) = delete;
  Emp& operator=(Emp&&) = delete;
};


#endif /* EMP_HPP_INCLUDED */
