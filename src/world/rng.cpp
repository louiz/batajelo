#include <world/rng.hpp>

Rng::Rng():
  seed_value(0),
  engine()
{
}

void Rng::seed(const unsigned int seed)
{
  this->seed_value = seed;
  this->engine.seed(seed);
}

void Rng::seed()
{
  std::random_device rd;
  this->seed_value = rd();
  this->engine.seed(this->seed_value);
}

unsigned long Rng::get_seed() const
{
  return this->seed_value;
}
