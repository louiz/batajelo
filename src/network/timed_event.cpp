#include <logging/logging.hpp>
#include <network/timed_event.hpp>
#include <network/timed_event_handler.hpp>

TimedEvent::TimedEvent(TimedEventHandler* handler,
		       boost::asio::deadline_timer* timer,
		       const t_timed_callback callback):
  handler(handler),
  timer(timer),
  callback(callback)
{
  this->timer->async_wait(boost::bind(&TimedEvent::on_expires, this, _1));
}

void TimedEvent::on_expires(const boost::system::error_code& error)
{
  if (error)
    return ;
  log_debug("on_timeout");
  this->callback();
  this->handler->remove_event(this);
}

TimedEvent::~TimedEvent()
{
  log_debug("Deleting timed event");
  delete this->timer;
}

void TimedEvent::cancel()
{
  this->timer->cancel();
}
