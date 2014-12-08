#ifndef MANA_COMPONENT_HPP_INCLUDED
#define MANA_COMPONENT_HPP_INCLUDED

#include <world/quantity.hpp>

class Mana: public Quantity
{
public:
  static const ComponentType component_type = ComponentType::Mana;

  Mana(const Quantity::type max);
  void tick(Entity*, World*) override final;

private:
  Mana(const Mana&) = delete;
  Mana(Mana&&) = delete;
  Mana& operator=(const Mana&) = delete;
  Mana& operator=(Mana&&) = delete;
};


#endif /* MANA_COMPONENT_HPP_INCLUDED */
