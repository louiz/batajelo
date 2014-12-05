#include <world/health.hpp>

const ComponentType Health::component_type;

Health::Health(const Health::type max):
  max(max),
  current(max)
{
}

Health::type Health::get() const
{
  return this->current;
}

Health::type Health::get_max() const
{
  return this->max;
}

void Health::set(const Health::type val)
{
  this->current = val;
}

Health::type Health::add(const Health::type val)
{
  this->current += val;
  return this->current;
}

void Health::confine()
{
  if (this->current < 0)
    this->current = 0;
  else if (this->current > this->max)
    this->current = max;
}
