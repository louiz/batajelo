#include <game/time.hpp>

long get_number_of_updates(Duration& duration)
{
  const long ms = duration.total_microseconds();
  if (ms < WORLD_UPDATE_DURATION)
      return 0;

  long updates = ms / WORLD_UPDATE_DURATION;
  duration -= boost::posix_time::microseconds(updates*WORLD_UPDATE_DURATION);
  return updates;
}

long get_number_of_graphicale_updates(Duration& duration)
{
  const long ms = duration.total_microseconds();
  if (ms < GRAPHICAL_UPDATE_DURATION)
      return 0;

  long updates = ms / GRAPHICAL_UPDATE_DURATION;
  duration -= boost::posix_time::microseconds(updates*GRAPHICAL_UPDATE_DURATION);
  return updates;
}

long sec(const Duration& dt)
{
  return dt.total_milliseconds();
}