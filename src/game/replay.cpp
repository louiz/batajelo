#include <game/replay.hpp>

#include <logging/logging.hpp>

Replay::Replay()
{
}

Replay::~Replay()
{
}

void Replay::insert_order(const Message& message)
{
  this->orders.emplace_back(message);
}

std::list<Message>& Replay::get_orders()
{
  return this->orders;
}
