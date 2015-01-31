#include <world/acquisition.hpp>

Acquisition::Acquisition(const Acquisition::type range):
  range(range)
{
}

Acquisition::type Acquisition::get_range() const
{
  return this->range;
}
