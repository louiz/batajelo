#include <world/abilities/blink.hpp>

Blink::Blink():
  ActiveAbility(TargetType::Point)
{
}

template<>
const std::string NamedAbility<Blink>::name = "Blink";
