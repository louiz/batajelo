#ifndef ACQUISITION_HPP_INCLUDED
#define ACQUISITION_HPP_INCLUDED

#include <world/components.hpp>
#include <fixmath/fix16.hpp>

/**
 * The ability for an entity to “notice” other entities
 */

class Acquisition: public Component
{
public:
  static const ComponentType component_type = ComponentType::Acquisition;
  using type = Fix16;
  Acquisition(const type range);
  ~Acquisition() = default;
  type get_range() const;

private:
  type range;

  Acquisition(const Acquisition&) = delete;
  Acquisition(Acquisition&&) = delete;
  Acquisition& operator=(const Acquisition&) = delete;
  Acquisition& operator=(Acquisition&&) = delete;
};

#endif /* ACQUISITION_HPP_INCLUDED */
