#ifndef NAMED_ABILITY_HPP_INCLUDED
#define NAMED_ABILITY_HPP_INCLUDED

#include <world/abilities/ability.hpp>

template<typename DerrivedClass>
class NamedAbility: public Ability
{
public:
  NamedAbility() = default;
  ~NamedAbility() = default;
  const std::string& get_name() const override final
    {
      return this->name;
    }

private:
  const static std::string name;

  NamedAbility(const NamedAbility&) = delete;
  NamedAbility(NamedAbility&&) = delete;
  NamedAbility& operator=(const NamedAbility&) = delete;
  NamedAbility& operator=(NamedAbility&&) = delete;
};


#endif /* NAMED_ABILITY_HPP_INCLUDED */

