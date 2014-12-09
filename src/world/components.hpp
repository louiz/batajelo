#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

class World;
class Entity;

enum class ComponentType
{
    Invalid,
    Location,
    Health,
    Mana,
    Vision,
};

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
