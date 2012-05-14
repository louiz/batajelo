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

class Action
{
public:
  Action();
  ~Action();
  void execute() const;

private:
  Action(const Action&);
  Action& operator=(const Action&);
};

#endif // __ACTION_HPP__
/**@}*/
