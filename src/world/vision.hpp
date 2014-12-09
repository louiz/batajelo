#ifndef VISION_COMPONENT_HPP_INCLUDED
#define VISION_COMPONENT_HPP_INCLUDED

#include <cstdint>

#include <world/components.hpp>

class Vision: public Component
{
public:
  static const ComponentType component_type = ComponentType::Vision;
  using type = uint16_t;

  Vision(const type range,
         const bool detection);
  void tick(Entity*, World*) override final;
  type get_range() const;

private:
  type range;
  bool detection;

  Vision(const Vision&) = delete;
  Vision(Vision&&) = delete;
  Vision& operator=(const Vision&) = delete;
  Vision& operator=(Vision&&) = delete;
};


#endif /* VISION_COMPONENT_HPP_INCLUDED */
