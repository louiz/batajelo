#include <logging/logging.hpp>
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
  const Action* action;
  os << "[" << turn.is_validated() << "]";
  for (const auto& action: turn.get_actions())
    os << *action;
  return os;
}

unsigned int Turn::get_number_of_validations() const
{
  return this->ready_clients.size();
}

std::vector<Action*>& Turn::get_actions()
{
  return this->actions;
}
