/** @addtogroup Game
 *  @{
 */

/**
 * A replay is just a list of actions associated with a turn number.
 * @class Replay
 */

#include <vector>

#ifndef __REPLAY_HPP__
# define __REPLAY_HPP__

#include <game/action.hpp>

class Replay
{
public:
  Replay();
  ~Replay();
  /**
   * Insert an action at the end of the action list.
   */
  void insert_action(Action*);
  std::vector<Action*>& get_actions();

private:
  Replay(const Replay&);
  Replay& operator=(const Replay&);

  std::vector<Action*> actions;
};

#endif // __REPLAY_HPP__
/**@}*/
