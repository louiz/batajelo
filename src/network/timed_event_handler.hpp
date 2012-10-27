/** @addtogroup Network
 *  @{
 */

/**
 * 
 * @class TimedEventHandler
 */

#include <map>

#ifndef __TIMED_EVENT_HANDLER_HPP__
# define __TIMED_EVENT_HANDLER_HPP__

#include <network/timed_event.hpp>

class TimedEventHandler
{
public:
  TimedEventHandler();
  ~TimedEventHandler();
  /**
   * Install a timed event to be executed in the futur at the specified date.
   */
  void install_timed_event(const t_timed_callback, const int);
  /**
   * Remove the event from the list. It is deleted and canceled (if it was
   * not already called, it'll never be).
   */
  void remove_event(TimedEvent*);
  /**
   * Returns a reference to the io_service object, so we can create
   * our timed_events on it.
   */
  virtual boost::asio::io_service& get_io_service() = 0;

private:
  TimedEventHandler(const TimedEventHandler&);
  TimedEventHandler& operator=(const TimedEventHandler&);

  std::vector<TimedEvent*> events;
};

#endif // __TIMED_EVENT_HANDLER_HPP__
/**@}*/
