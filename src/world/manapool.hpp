#ifndef MANAPOOL_COMPONENT_HPP_INCLUDED
#define MANAPOOL_COMPONENT_HPP_INCLUDED

#include <world/quantity.hpp>

class ManaPool: public Quantity
{
public:
  static const ComponentType component_type = ComponentType::ManaPool;

  ManaPool(const Quantity::type max);
  void tick(Entity*, World*) override final;

private:
  ManaPool(const ManaPool&) = delete;
  ManaPool(ManaPool&&) = delete;
  ManaPool& operator=(const ManaPool&) = delete;
  ManaPool& operator=(ManaPool&&) = delete;
};


#endif /* MANAPOOL_COMPONENT_HPP_INCLUDED */
