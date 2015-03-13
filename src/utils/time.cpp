#include <utils/time.hpp>

namespace utils
{

  Time now()
  {
    return std::chrono::steady_clock::now();
  }

  unsigned long get_number_of_ticks(std::chrono::steady_clock::duration& duration)
  {
    if (duration < tick_duration)
      return 0;

    unsigned long updates = Duration{duration / tick_duration}.count();
    duration -= tick_duration * updates;
    return updates;
  }

  FloatingSeconds sec(const Duration& dt)
  {
    return std::chrono::duration_cast<FloatingSeconds>(dt);
  }

  FloatingSeconds passed_time()
  {
    Duration t = now() - start_time;
    return sec(t);
  }
}
