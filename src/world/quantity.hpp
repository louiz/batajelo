#ifndef QUANTITY_COMPONENT_HPP_INCLUDED
#define QUANTITY_COMPONENT_HPP_INCLUDED

#include <world/components.hpp>

#include <fixmath/fix16.hpp>
#include <cstdint>

class Quantity: public Component
{
public:
  using type = Fix16;

  Quantity(const type max);
  ~Quantity() = default;
  type get() const;
  type get_max() const;
  void set(const type val);
  void set_max(const type val);
  type add(const type val);

private:
  /**
   * Make sure the value doesn't go off-limit.
   */
  void confine();
  type max;
  type current;

  Quantity(const Quantity&) = delete;
  Quantity(Quantity&&) = delete;
  Quantity& operator=(const Quantity&) = delete;
  Quantity& operator=(Quantity&&) = delete;
};

#endif /* QUANTITY_COMPONENT_HPP_INCLUDED */
