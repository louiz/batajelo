#include <game/action.hpp>

Action::Action(t_action_callback callback,
	       ActionEvent* event,
	       unsigned int validations_needed):
  validations_needed(validations_needed),
  completely_validated(false),
  event(event),
  callback(callback)
{
}

Action::~Action()
{
  log_debug("delete action");
  delete this->event;
}

void Action::execute() const
{
  if (this->callback != 0)
    this->callback(this->event);
}

bool Action::is_validated() const
{
  log_debug("Action::is_validated(). Validation needed: " << this->validations_needed << ". Validation done: " << this->ready_clients.size());
  assert(this->ready_clients.size() <= this->validations_needed);

  if (this->ready_clients.size() == this->validations_needed)
    return true;
  return false;
}

bool Action::validate(const unsigned long int id)
{
  // debug:
  std::vector<unsigned long int>::const_iterator it;
  for (it = this->ready_clients.begin(); it < this->ready_clients.end(); ++it)
    assert((*it) != id);
  // end debug.
  this->ready_clients.push_back(id);
  if (this->is_validated() == true)
    {
      this->validate_completely();
      return true;
    }
  return false;
}

unsigned long int Action::get_id() const
{
  return this->event->get_id();
}

void Action::validate_completely()
{
  log_debug("Action becomes completely_validated: " << this->get_id());
  assert(this->completely_validated == false);
  this->completely_validated = true;
}

bool Action::is_completely_validated() const
{
  return this->completely_validated;
}

ActionEvent* Action::get_event() const
{
  return this->event;
}

std::ostream& operator<<(std::ostream& os, const Action& action)
{
  os << "action(" << action.ready_clients.size() << "/" << action.validations_needed << ")-> " << action.completely_validated;
  return os;
}

void Action::set_validations_needed(unsigned int number)
{
  this->validations_needed = number;
}
