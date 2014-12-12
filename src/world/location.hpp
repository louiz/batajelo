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
  Fix16 get_width() const;
  void set_width(const Fix16& width);

private:
  Position pos;
  Fix16 width;

  Location(const Location&) = delete;
  Location(Location&&) = delete;
  Location& operator=(const Location&) = delete;
  Location& operator=(Location&&) = delete;
};


#endif /* LOCATION_COMPONENT_HPP_INCLUDED */
