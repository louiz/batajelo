#include <world/time.hpp>

long get_number_of_updates(Duration& duration)
{
  const long ms = duration.total_microseconds();
  if (ms < WORLD_UPDATE_DURATION)
      return 0;

  long updates = ms / WORLD_UPDATE_DURATION;
  duration -= boost::posix_time::microseconds(updates*WORLD_UPDATE_DURATION);
  return updates;
}

float sec(const Duration& dt)
{
  return (float)dt.total_milliseconds() / 1000.f;
}
