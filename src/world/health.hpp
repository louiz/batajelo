#ifndef HEALTH_COMPONENT_HPP_INCLUDED
#define HEALTH_COMPONENT_HPP_INCLUDED

#include <world/components.hpp>
#include <fixmath/fix16.hpp>
#include <cstdint>

class Health: public Component
{
public:
  using type = Fix16;
  static const ComponentType component_type = ComponentType::Health;

  Health(const type max);
  ~Health() = default;
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

  Health(const Health&) = delete;
  Health(Health&&) = delete;
  Health& operator=(const Health&) = delete;
  Health& operator=(Health&&) = delete;
};

#endif /* HEALTH_COMPONENT_HPP_INCLUDED */
