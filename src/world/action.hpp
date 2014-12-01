/**
 * An action, associated with a turn, kept in a turn_handler, with a
 * callback executed when the turn is passed (see TurnHandler).
 */

#ifndef __ACTION_HPP__
# define __ACTION_HPP__

#include <functional>

using Action = std::function<void(void)>;

#endif // __ACTION_HPP__
