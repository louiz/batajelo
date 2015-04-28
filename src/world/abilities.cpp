#include <world/entity.hpp>
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

template <typename T>
T* get_ability(Entity* entity)
{
  Abilities* abilities = entity->get<Abilities>();
  if (!abilities)
    return nullptr;
  Ability* ability = abilities->find(T::ability_type);
  if (!ability)
    return nullptr;
  return static_cast<T*>(ability);
}

Ability* get_ability(Entity* entity, const AbilityType& type)
{
  Abilities* abilities = entity->get<Abilities>();
  if (!abilities)
    return nullptr;
  return abilities->find(type);
}
