#ifndef COMPONENT_HPP_INCLUDED
#define COMPONENT_HPP_INCLUDED

class Component
{
public:
  Component() = default;
  virtual ~Component() = default;

private:
  Component(const Component&) = delete;
  Component(Component&&) = delete;
  Component& operator=(const Component&) = delete;
  Component& operator=(Component&&) = delete;
};

enum class ComponentType
{
  Location,
    Health,
};

#endif /* COMPONENT_HPP_INCLUDED */
