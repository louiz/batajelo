/** @addtogroup Game
 *  @{
 */

/**
 * An action, associated with a turn, kept in a turn_handler, with a
 * callback executed when the turn is passed (see TurnHandler).
 * @class Action
 */

#include <boost/function.hpp>

#ifndef __ACTION_HPP__
# define __ACTION_HPP__

#include <logging/logging.hpp>

typedef boost::function<void(void)> t_action_callback;

class Action
{
public:
  Action(t_action_callback);
  ~Action();
  void execute() const;

private:
  Action(const Action&);
  Action& operator=(const Action&);

  t_action_callback callback;
};


#endif // __ACTION_HPP__
/**@}*/
