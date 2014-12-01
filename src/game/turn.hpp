#ifndef __TURN_HPP__
# define __TURN_HPP__

#include <world/action.hpp>
#include <vector>
#include <memory>

class Turn
{
public:
  Turn();
  Turn(Turn&& o) = default;
  Turn& operator=(Turn&&) = default;
  ~Turn();

  /**
   * Insert a new Action, that will be executed by execute()
   */
  void insert(Action&& action);
  /**
   * Execute all contained Actions
   */
  void execute();
  /**
   * Mark the turn as ready to be executed
   */
  void mark_ready();
  /**
   * Is the turn ready to be executed
   */
  bool is_ready() const;

  const std::vector<Action>& get_actions() const;

private:
  std::vector<Action> actions;
  bool ready;

  Turn& operator=(const Turn&) = delete;
  Turn(const Turn&) = delete;
};


#endif // __TURN_HPP__
