#include "boost/date_time/posix_time/posix_time.hpp"

#ifndef __TIME_HPP__
# define __TIME_HPP__

#include <logging/logging.hpp>

/**
 * The number of microseconds for each world update.
 * a lower number makes everything more "fluid" (to a certain
 * point where the difference would not even be noticed), but
 * requires more CPU speed.
 */
#define WORLD_UPDATE_DURATION 10000

typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration Duration;

/**
 * Takes a duration, make it % WORLD_UPDATE_DURATION
 * and returns the number of WORLD_UPDATE_DURATION it contains.
 * i.e., if WORLD_UPDATE_DURATION is 100 and the given duration is
 * 245 ms, the duration becomes 45 and the function returns 2.
 */
long get_number_of_updates(Duration& duration);

float sec(const Duration&);

#endif // __TIME_HPP__
