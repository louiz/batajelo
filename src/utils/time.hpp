#ifndef UTILS_TIME_HPP_INCLUDED
# define UTILS_TIME_HPP_INCLUDED

#include <chrono>

using namespace std::chrono_literals;

namespace utils
{
  using namespace std::chrono_literals;

  // A timepoint, gotten from a steady_clock because we don't want time to
  // go backward during the game
  using Time = std::chrono::time_point<std::chrono::steady_clock>;

  // A duration is expressed in microseconds.
  using Duration = std::chrono::microseconds;

  using FloatingSeconds = decltype(0.1s);

  /**
   * Returns the current time, from a steady clock
   */
  Time now();

  /**
   * Takes a duration, makes it “% tick_duration” and returns the number of
   * tick_duration it contains.

   * i.e. if tick_duration is 100 and the given duration is 245us, the
   * duration becomes 45 and the function returns 2: because the given
   * duration contains 2 whole ticks, and we keep the remaining, to be
   * consumed later on.
   */
  unsigned long get_number_of_ticks(Duration& duration);

  /**
   * Convert a duration into a (float point) seconds.

   * e.g 2'500'000 is converted into 2.5 seconds
   */
  FloatingSeconds sec(const Duration&);
}

#endif // UTILS_TIME_HPP_INCLUDED
