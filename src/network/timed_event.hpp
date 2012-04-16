/** @addtogroup Network
 *  @{
 */

/**
 * An event to be executed (using a callback) in the future, after the
 * specified time has passed.
 * @class TimedEvent
 */

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#ifndef __TIMED_EVENT_HPP__
# define __TIMED_EVENT_HPP__

class TimedEventHandler;

typedef boost::function< void(void) > t_timed_callback;

class TimedEventHandler;

class TimedEvent
{
public:
  TimedEvent(const TimedEventHandler*,
	     boost::asio::deadline_timer*,
	     const t_timed_callback);
  ~TimedEvent();
  void cancel();
  void on_expires(const boost::system::error_code&);

private:
  TimedEvent(const TimedEvent&);
  TimedEvent& operator=(const TimedEvent&);

  const TimedEventHandler* handler;
  boost::asio::deadline_timer* timer;
  const t_timed_callback callback;
};

#endif // __TIMED_EVENT_HPP__
