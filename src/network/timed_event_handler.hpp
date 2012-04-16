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
#include <logging/logging.hpp>

class TimedEventHandler
{
public:
  TimedEventHandler();
  ~TimedEventHandler();
  /**
   * Install a timed event to be executed in the futur at the specified date.
   */
  void install_timed_event(const t_timed_callback, const int);

  virtual boost::asio::io_service& get_io_service() = 0;

private:
  TimedEventHandler(const TimedEventHandler&);
  TimedEventHandler& operator=(const TimedEventHandler&);

  std::vector<TimedEvent*> events;
};

#endif // __TIMED_EVENT_HANDLER_HPP__
/**@}*/
