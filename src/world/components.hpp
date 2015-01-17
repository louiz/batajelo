#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

#include <vector>

class World;
class Entity;

enum class ComponentType: std::size_t
{
    Invalid,
    Location,
    Mobility,
    Health,
    ManaPool,
    Vision,
    Team,
    Abilities,

    count
};

constexpr std::size_t ComponentTypeCount = static_cast<std::size_t>(ComponentType::count);

class Component
{
public:
  static const ComponentType component_type = ComponentType::Invalid;

  Component() = default;
  virtual ~Component() = default;
  virtual void tick(Entity*, World*) = 0;

private:
  Component(const Component&) = delete;
  Component(Component&&) = delete;
  Component& operator=(const Component&) = delete;
  Component& operator=(Component&&) = delete;
};

#endif /* COMPONENT_HPP_INCLUDED */
