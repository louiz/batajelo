#include <world/abilities.hpp>

#include <cassert>

const std::vector<std::unique_ptr<Ability>>& Abilities::get() const
{
  return this->abilities;
}

const Ability* Abilities::get(const std::size_t index) const
{
  if (index < this->abilities.size())
    return this->abilities[index].get();
  return nullptr;
}

void Abilities::add(const std::size_t index,
                    std::unique_ptr<Ability> ability)
{
  this->abilities[index] = std::move(ability);
}
