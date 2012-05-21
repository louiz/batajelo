/** @addtogroup Game
 *  @{
 */

/**
 * An action, associated with a turn, kept in a turn_handler, with a
 * callback executed when the turn is passed (see TurnHandler).
 * @class Action
 */

#include <boost/function.hpp>
#include <vector>

#ifndef __ACTION_HPP__
# define __ACTION_HPP__

#include <logging/logging.hpp>
#include <game/event.hpp>

namespace actions
{
  enum Type
    {
      Select,
      Move,
      Attack,
      Cast,

      None
    };
}

typedef boost::function< void(Event*) > t_action_callback;

class Action
{
public:
  Action(t_action_callback, Event*, unsigned int);
  ~Action();
  void execute() const;
  /**
   * Returns true if the action is validated. This
   * means that the number of ready_clients is the same
   * as the validations_needed.
   */
  bool is_validated() const;
  /**
   * Validate the action for one client (given its id).
   */
  void validate(const unsigned long int);
  unsigned long int get_id() const;

private:
  Action(const Action&);
  Action& operator=(const Action&);
  /**
   * The list of the clients' IDs that confirmed this action.
   * When we want to execute the action, we check that it
   * has been confirmed by EVERY other client, if not, we pause the game.
   */
public:
  std::vector<unsigned long int> ready_clients;
  /**
   * The number of validation needed for this action to be totally
   * validated (and therefore executed).
   */
  unsigned int validations_needed;

  /**
   * The structure passed to the callback when we execute the action.
   */
  Event* event;
  t_action_callback callback;
};

#endif // __ACTION_HPP__
/**@}*/
