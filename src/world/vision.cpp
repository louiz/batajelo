#include <world/vision.hpp>

const ComponentType Vision::component_type;

Vision::Vision(const Vision::type range,
               const bool detection):
  range(range),
  detection(detection)
{
}

void Vision::tick(Entity*,  World*)
{
}

Vision::type Vision::get_range() const
{
  return this->range;
}
