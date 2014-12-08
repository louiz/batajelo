#include <world/quantity.hpp>

Quantity::Quantity(const Quantity::type max):
  max(max),
  current(max)
{
}

Quantity::type Quantity::get() const
{
  return this->current;
}

Quantity::type Quantity::get_max() const
{
  return this->max;
}

void Quantity::set(const Quantity::type val)
{
  this->current = val;
  this->confine();
}

Quantity::type Quantity::add(const Quantity::type val)
{
  this->current += val;
  this->confine();
  return this->current;
}

void Quantity::confine()
{
  if (this->current < 0)
    this->current = 0;
  else if (this->current > this->max)
    this->current = max;
}
