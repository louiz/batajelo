#ifndef MOBILITY_COMPONENT_HPP_INCLUDED
#define MOBILITY_COMPONENT_HPP_INCLUDED

#include <world/components.hpp>
#include <fixmath/fix16.hpp>
#include <world/path.hpp>

#include <cstdint>

class Location;
class World;

class Mobility: public Component
{
public:
  static const ComponentType component_type = ComponentType::Mobility;

  Mobility(const Fix16& speed);
  ~Mobility() = default;
  void tick(Entity*, World*) override final;
  Fix16 get_speed() const;
  void follow_path(Path& path, World* world, Location* location);

private:
  Fix16 speed;

  Mobility(const Mobility&) = delete;
  Mobility(Mobility&&) = delete;
  Mobility& operator=(const Mobility&) = delete;
  Mobility& operator=(Mobility&&) = delete;
};


#endif /* MOBILITY_COMPONENT_HPP_INCLUDED */
