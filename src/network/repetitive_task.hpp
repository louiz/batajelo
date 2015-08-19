#ifndef REPETITIVE_TASK_HPP_INCLUDED
#define REPETITIVE_TASK_HPP_INCLUDED

#include <chrono>
#include <functional>
#include <boost/asio/steady_timer.hpp>

/**
 * Calls the given function every interval of time, until the function
 * returns false.
 */

class Repeater
{
  using FuncType = std::function<bool()>;
  using TimerType = boost::asio::steady_timer;
  using DurationType = std::chrono::milliseconds;

public:
  Repeater(const FuncType& f, const DurationType& duration);
  ~Repeater() = default;

private:
  void repeat_forever();

  const FuncType f;
  TimerType timer;
  const DurationType duration;

  Repeater(const Repeater&) = delete;
  Repeater(Repeater&&) = delete;
  Repeater& operator=(const Repeater&) = delete;
  Repeater& operator=(Repeater&&) = delete;
};

#endif /* REPETITIVE_TASK_HPP_INCLUDED */
