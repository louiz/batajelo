/** @addtogroup Network
 *  @{
 */

/**
 * @class TimedEventHandler
 */

#ifndef __TIMED_EVENT_HANDLER_HPP__
# define __TIMED_EVENT_HANDLER_HPP__

#include <map>

#include <boost/asio.hpp>

#include <network/timed_event.hpp>

class TimedEventHandler
{
public:
  explicit TimedEventHandler();
  ~TimedEventHandler();
  /**
   * Install a timed event to be executed in the futur at the specified date.
   */
  void install_timed_event(boost::asio::io_service&, const t_timed_callback, const int);
  /**
   * Remove the event from the list. It is deleted and canceled (if it was
   * not already called, it'll never be).
   */
  void remove_event(TimedEvent*);

private:
  TimedEventHandler(const TimedEventHandler&);
  TimedEventHandler& operator=(const TimedEventHandler&);

  std::vector<TimedEvent*> events;
};

#endif // __TIMED_EVENT_HANDLER_HPP__
/**@}*/
