#ifndef HEALTH_COMPONENT_HPP_INCLUDED
#define HEALTH_COMPONENT_HPP_INCLUDED

#include <world/quantity.hpp>

class Health: public Quantity
{
public:
  static const ComponentType component_type = ComponentType::Health;

  Health(const Quantity::type max);
  void tick(Entity*, World*) override final;

private:
  Health(const Health&) = delete;
  Health(Health&&) = delete;
  Health& operator=(const Health&) = delete;
  Health& operator=(Health&&) = delete;
};

#endif /* HEALTH_COMPONENT_HPP_INCLUDED */
