#include <logging/logging.hpp>
#include <network/timed_event_handler.hpp>

TimedEventHandler::TimedEventHandler()
{
}

TimedEventHandler::~TimedEventHandler()
{
  log_debug("Deleting TimedEventHandler. " << this->events.size() << " events were pending.");
  std::vector<TimedEvent*>::iterator it;
  for (it = this->events.begin(); it < this->events.end(); ++it)
    {
      (*it)->cancel();
      delete (*it);
    }
}

void TimedEventHandler::install_timed_event(boost::asio::io_service& io_service,
                                            const timed_callback_t callback,
                                            const int delay)
{
  log_debug("installing timed_event");
  boost::asio::deadline_timer* timer = new boost::asio::deadline_timer(io_service, boost::posix_time::seconds(delay));
  TimedEvent* event = new TimedEvent(this, timer, callback);
  this->events.push_back(event);
}

void TimedEventHandler::remove_event(TimedEvent* event)
{
  std::vector<TimedEvent*>::iterator it;
  for (it = this->events.begin(); it < this->events.end(); ++it)
    {
      if ((*it) == event)
     {
       (*it)->cancel();
       delete (*it);
       this->events.erase(it);
       return ;
     }
    }

}
