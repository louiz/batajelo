#include <world/manapool.hpp>

const ComponentType ManaPool::component_type;

ManaPool::ManaPool(const Quantity::type max):
  Quantity(max)
{
}

void ManaPool::tick(Entity*,  World*)
{
  this->add(0.01);
}
