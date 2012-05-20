#ifndef __TURN_HPP__
# define __TURN_HPP__

#include <game/action.hpp>

class Turn
{
public:
  ~Turn();
  void execute();
  void insert(Action*);
  bool is_validated() const;
  Action* get_next_action();
  void reset_action_iterator();

private:
  std::vector<Action*> actions;
  std::vector<Action*>::const_iterator actions_iterator;
};


#endif // __TURN_HPP__
