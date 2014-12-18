#ifndef LOCATION_COMPONENT_HPP_INCLUDED
#define LOCATION_COMPONENT_HPP_INCLUDED

#include <world/components.hpp>
#include <world/position.hpp>

#include <cstdint>

class Location: public Component
{
public:
  static const ComponentType component_type = ComponentType::Location;
  Location(const uint8_t width);
  ~Location() = default;
  void tick(Entity*, World*) override final;
  Position& position();
  const Position& position() const;
  Fix16 get_width() const;
  void set_width(const Fix16& width);
  bool contains(const Position&) const;
  /**
   * Returns weither or not this entity (if not moving) makes it impossible
   * for the given unit to reach this Position. This means that this entity
   * is OVER the position, or close enough that the moving entity cannot
   * reach it because of its width.
   */
  bool is_obstructing_position(const Position&) const;

private:
  Fix16 width;
  Position pos;

  Location(const Location&) = delete;
  Location(Location&&) = delete;
  Location& operator=(const Location&) = delete;
  Location& operator=(Location&&) = delete;
};


#endif /* LOCATION_COMPONENT_HPP_INCLUDED */
