/**
 *
 */

#ifndef __TURN_HANDLER_HPP__
# define __TURN_HANDLER_HPP__

#include <deque>
#include <memory>
#include <cstdint>
#include <functional>

#include <game/turn.hpp>
#include <world/action.hpp>

using TurnNb = std::size_t;
using t_next_turn_callback = std::function< void(const TurnNb)>;

/**
 * The number of ticks contained in a turn
 */
static constexpr std::size_t TURN_TICKS = 10;

class TurnHandler
{
public:
  TurnHandler();
  ~TurnHandler();
  /**
   * Advance the current turn_advancement, and if the TURN_TIME is reached,
   * we call next_turn, which in turn executes the commands associated with
   * this turn.
   */
  void tick();
  /**
   * Insert an action element into the appropriate turn vector
   * (which will contain one or more action).
   */
  void insert_action(Action&& action, const TurnNb nb);
  /**
   * Return true if the next turn has been fully validated, false otherwise.
   */
  bool is_next_turn_ready() const;
  /**
   * Find the next turn that is not marked as ready (create it, if it
   * doesn’t exist), and mark it as ready.
   */
  void mark_turn_as_ready();
  /**
   * Mark all turns as ready until (and including) the nth
   */
  void mark_as_ready_until(const TurnNb n);

  void set_next_turn_callback(t_next_turn_callback callback);

  bool is_paused() const;
  /**
   * Returns the turn we are currently at.
   */
  TurnNb get_current_turn() const;
  std::deque<Turn>& get_turns();

private:
  TurnHandler(const TurnHandler&);
  TurnHandler& operator=(const TurnHandler&);

  /**
   * Change the state to paused.
   */
  void pause();
  /**
   * Change the state to unpaused.
   */
  void unpause();
  /**
   * Go to the next turn, and executes the action
   * associated with this turn.
   */
  void next_turn();
  /**
   * Insert a turn.  If the turn already exists, does nothing.
   */
  void insert_turn(const TurnNb nb);

  std::deque<Turn> turns;
  unsigned long current_turn;
  unsigned int turn_advancement;

  bool paused;

  /**
   * A function that will be called whenever we go to the next turn.
   */
  t_next_turn_callback next_turn_callback;
};

#endif // __TURN_HANDLER_HPP__
