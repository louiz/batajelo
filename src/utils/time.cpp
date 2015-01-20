#include <utils/time.hpp>

namespace utils
{

  // The duration of a world tick, in microseconds
  constexpr Duration tick_duration{10000us};

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

}
