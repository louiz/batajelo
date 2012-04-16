#include <network/ping_handler.hpp>

PingHandler::PingHandler():
  last_ping_date(microsec_clock::universal_time()),
  latency(0, 0, 0, 0)
{
}

PingHandler::~PingHandler()
{
}

void PingHandler::ping_sent()
{
  this->last_ping_date = microsec_clock::universal_time();
}

void PingHandler::pong_received()
{
  ptime current_time(microsec_clock::universal_time());
  this->latency = current_time - this->last_ping_date;
}

long PingHandler::get_latency()
{
  return this->latency.total_microseconds();
}
