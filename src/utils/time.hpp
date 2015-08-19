#ifndef UTILS_TIME_HPP_INCLUDED
# define UTILS_TIME_HPP_INCLUDED

#include <chrono>

using namespace std::chrono_literals;

namespace utils
{
  // A timepoint, gotten from a steady_clock because we don't want time to
  // go backward during the game
  using Time = std::chrono::time_point<std::chrono::steady_clock>;

  // A duration is expressed in nano.
  using Duration = std::chrono::steady_clock::duration;

  using FloatingSeconds = decltype(0.1s);

  // This means that there are 50 ticks per seconds.  We can use
  // std::duration_cast to convert from std::chrono::seconds to utils::ticks
  // for example.  This way we can simply express a duration in seconds in
  // the abilities/entities description, and convert that into a number of
  // ticks in the lower-level code
  using ticks = std::chrono::duration<int, std::ratio<1, 50>>;

  // The duration of a world tick, in microseconds
  constexpr Duration tick_duration = std::chrono::duration_cast<Duration>(ticks(1));

  std::size_t duration_to_ticks(const Duration& duration);

  /**
   * Returns the current time, from a steady clock
   */
  Time now();

  /**
   * Takes a duration, makes it “% tick_duration” and returns the number of
   * tick_duration it contains.

   * e.g. if tick_duration is 100us and the given duration is 245us, the
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

  static Time start_time = now();
  FloatingSeconds passed_time();
}

#endif // UTILS_TIME_HPP_INCLUDED
