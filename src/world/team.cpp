#include <world/team.hpp>

const ComponentType Team::component_type;

Team::Team(const Team::type number):
  number(number)
{
}

Team::Team():
  number(0)
{
}

Team::type Team::get() const
{
  return this->number;
}

void Team::set(const Team::type number)
{
  this->number = number;
}

void Team::tick(Entity* entity, World* world)
{
}
