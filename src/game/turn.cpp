#include <game/turn.hpp>

Turn::~Turn()
{
  std::vector<Action*>::iterator action_it;
  for (action_it = this->actions.begin(); action_it < this->actions.end(); ++action_it)
    {
      delete (*action_it);
    }
  this->actions.clear();
}

void Turn::execute()
{
  log_debug(this->actions.size() << " actions to execute");
  std::vector<Action*>::iterator it;
  for (it = this->actions.begin(); it < this->actions.end(); ++it)
    {
      (*it)->execute();
      delete (*it);
    }
  this->actions.clear();
}

void Turn::insert(Action* action)
{
  this->actions.push_back(action);
}

bool Turn::is_validated() const
{
  std::vector<Action*>::const_iterator it;
  for (it = this->actions.begin(); it < this->actions.end(); ++it)
    {
      if ((*it)->is_completely_validated() == false)
  	return false;
    }
  return true;
}

Action* Turn::get_next_action()
{
  if (this->actions_iterator == this->actions.end())
    {
      this->actions_iterator = this->actions.begin();
      return 0;
    }
  Action* action = *this->actions_iterator;
  ++this->actions_iterator;
  return action;
}

void Turn::reset_action_iterator()
{
  this->actions_iterator = this->actions.begin();
}
