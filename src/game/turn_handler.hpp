/** @addtogroup Game
 *  @{
 */

/**
 *
 * @class TurnHandler
 */

#include <vector>

#ifndef __TURN_HANDLER_HPP__
# define __TURN_HANDLER_HPP__

#include <logging/logging.hpp>
#include <game/action.hpp>

/**
 * The number of ticks contained in a turn
 */
#define TURN_TIME 10

class TurnHandler
{
public:
  TurnHandler();
  ~TurnHandler();
  /**
   * Advance the current turn_advancement, and if the TURN_TIME
   * is elapsed, we call next_turn, which in turn executes the commands
   * associated with this turn.
   */
  void tick();
  /**
   * Go to the next turn, and executes the action
   * associated with this turn.
   */
  void next_turn();
  /**
   * Insert an action element into the appropriate turn vector
   * (which will contain one or more action)
   */
  bool insert_action(const Action*, unsigned long turn);

private:
  TurnHandler(const TurnHandler&);
  TurnHandler& operator=(const TurnHandler&);

  std::deque<std::vector<const Action*> > turns;
  unsigned long current_turn;
  unsigned int turn_advancement;
};

#endif // __TURN_HANDLER_HPP__
/**@}*/
