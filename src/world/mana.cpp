#include <world/mana.hpp>

const ComponentType Mana::component_type;

Mana::Mana(const Quantity::type max):
  Quantity(max)
{
}

void Mana::tick(Entity*,  World*)
{
  this->add(0.01);
}
