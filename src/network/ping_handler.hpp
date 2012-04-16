/** @addtogroup Network
 *  @{
 */

/**
 * Stores the ping (request) time and gets the ping latency when
 * the pong is received by comparing both dates.
 * Just call PingHandler::ping_sent() when you are sending the ping request,
 * and PingHandler::pong_received() when you receive the answer. Afterward, you
 * can retrieve the current latency using PingHandler::get_latency().
 * @class CommandHandler
 */

#include <boost/date_time/posix_time/posix_time.hpp>

#ifndef __PING_HANDLER_HPP__
# define __PING_HANDLER_HPP__

using namespace boost::posix_time;

class PingHandler
{
public:
  PingHandler();
  ~PingHandler();
  void ping_sent();
  void pong_received();
  long get_latency();

private:
  PingHandler(const PingHandler&);
  PingHandler& operator=(const PingHandler&);
  /**
   * The time at which we sent the last ping request.
   */
  ptime last_ping_date;
  time_duration latency;
};

#endif // __PING_HANDLER_HPP__
/**@}*/
