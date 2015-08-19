#ifndef COOLDOWN_HPP_INCLUDED
#define COOLDOWN_HPP_INCLUDED

#include <utils/time.hpp>

/**
 * Defines a simple class that tells when an ability is ready. It’s ready
 * when the current value is 0.  When the ability is used, call start(). An
 * optional value can be passed to set it to a specific value instead of its
 * maximum. It can’t be set above the maximum, though.
 * Calling refresh() makes the ability ready by setting the current value to 0.
 */

class Cooldown
{
public:
  Cooldown():
    max(0),
    current(0)
  {}
  ~Cooldown() = default;
  void refresh()
  {
    this->current = 0;
  }
  void set_max(const utils::Duration& duration)
  {
    this->set_max(utils::duration_to_ticks(duration));
  }
  void set_max(const std::size_t ticks)
  {
    this->max = ticks;
  }
  void start(const std::size_t val=0)
  {
    if (val == 0)
      this->current = this->max;
    else
      this->current = std::min(val, this->max);
  }
  bool is_ready() const
  {
    return this->current == 0;
  }
  void tick()
  {
    if (this->current)
      this->current--;
  }
  std::size_t get_max_in_ticks() const
  {
    return this->max;
  }
  utils::Duration get_max_duration() const
  {
    return utils::tick_duration * this->get_max_in_ticks();
  }
  std::size_t get_remaining_ticks() const
  {
    return this->current;
  }
  utils::Duration get_remaining_duration() const
  {
    return utils::tick_duration * this->get_remaining_ticks();
  }

  std::size_t max;
  std::size_t current;

private:
  Cooldown(const Cooldown&) = delete;
  Cooldown(Cooldown&&) = delete;
  Cooldown& operator=(const Cooldown&) = delete;
  Cooldown& operator=(Cooldown&&) = delete;
};


#endif /* COOLDOWN_HPP_INCLUDED */
