/** @addtogroup Game
 *  @{
 */

/**
 *
 * @class TurnHandler
 */

#include <deque>

#ifndef __TURN_HANDLER_HPP__
# define __TURN_HANDLER_HPP__

#include <logging/logging.hpp>
#include <game/turn.hpp>
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
   * Insert an action element into the appropriate turn vector
   * (which will contain one or more action)
   */
  bool insert_action(Action*, const unsigned long turn);
  /**
   * Return true if the next turn has been fully validated, false otherwise.
   */
  bool is_next_turn_validated() const;
  /**
   * Change the state to paused.
   */
  void pause();
  /**
   * Change the state to unpaused.
   */
  void unpause();

  /**
   * Returns the next available Action object from the currently retrieved
   * turn.
   */
  Turn* get_turn(const unsigned int);
  /**
   * Search the action by its id, and validate it.
   * Returns false if the action is not found.
   */
  bool validate_action(const unsigned int id, const unsigned long int by);

  /**
   * Returns the turn we are currently at.
   */
  unsigned long get_current_turn();

private:
  TurnHandler(const TurnHandler&);
  TurnHandler& operator=(const TurnHandler&);

  /**
   * Go to the next turn, and executes the action
   * associated with this turn.
   */
  void next_turn();

  std::deque<Turn> turns;
  unsigned long current_turn;
  unsigned int turn_advancement;

  bool paused;
};

#endif // __TURN_HANDLER_HPP__
/**@}*/
