/** @addtogroup Network
 *  @{
 */

/**
 * An event to be executed (using a callback) in the future, after the
 * specified time has passed.
 * @class TimedEvent
 */

#ifndef __TIMED_EVENT_HPP__
# define __TIMED_EVENT_HPP__

#include <functional>

#include <boost/asio.hpp>

class TimedEventHandler;

typedef std::function< void(void) > timed_callback_t;

class TimedEventHandler;

class TimedEvent
{
public:
  TimedEvent(TimedEventHandler* handler,
	     boost::asio::deadline_timer* timer,
	     const timed_callback_t callback);
  ~TimedEvent();
  void cancel();
  void on_expires(const boost::system::error_code&);

private:
  TimedEvent(const TimedEvent&);
  TimedEvent& operator=(const TimedEvent&);

  TimedEventHandler* handler;
  boost::asio::deadline_timer* timer;
  const timed_callback_t callback;
};

#endif // __TIMED_EVENT_HPP__
