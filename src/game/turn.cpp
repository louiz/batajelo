#include <game/turn.hpp>

Turn::Turn():
  validated(false)
{
}

Turn::~Turn()
{
  std::vector<Action*>::iterator action_it;
  for (action_it = this->actions.begin(); action_it < this->actions.end(); ++action_it)
    {
      delete (*action_it);
    }
  this->actions.clear();
}

void Turn::execute(bool delete_actions)
{
  log_debug(this->actions.size() << " actions to execute");
  std::vector<Action*>::iterator it;
  for (it = this->actions.begin(); it < this->actions.end(); ++it)
    {
      log_warning("Executing one action");
      (*it)->execute();
      if (delete_actions)
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
  if (this->validated == true)
    return true;
  return false;
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

bool Turn::validate(const unsigned long int by,
                    const unsigned int confirmations_needed)
{
  if (this->validated == true)
    return false;
  this->ready_clients.push_back(by);
  if (this->ready_clients.size() >= confirmations_needed)
    {
      this->validate_completely();
      return true;
    }
  return false;
}

void Turn::validate_completely()
{
  this->validated = true;
}

std::ostream& operator<<(std::ostream& os, Turn& turn)
{
  turn.reset_action_iterator();
  const Action* action;
  os << "[" << turn.is_validated() << "]";
  while ((action = turn.get_next_action()) != 0)
    os << *action;
  return os;
}
