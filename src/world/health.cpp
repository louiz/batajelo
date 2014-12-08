#include <world/health.hpp>

const ComponentType Health::component_type;

Health::Health(const Quantity::type max):
  Quantity(max)
{
}

void Health::tick(Entity*, World*)
{
}
