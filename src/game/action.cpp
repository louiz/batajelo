#include <game/action.hpp>

Action::Action(t_action_callback callback, Event* event, unsigned int validations_needed):
  validations_needed(validations_needed),
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
  assert(this->ready_clients.size() <= this->validations_needed);

  // log_debug("Action::is_validated(). Validation needed: " << this->validations_needed << ". Validation done: " << this->ready_clients.size());
  if (this->ready_clients.size() == this->validations_needed)
    return true;
  return false;
}

void Action::validate(const unsigned long int id)
{
  // debug:
  std::vector<unsigned long int>::const_iterator it;
  for (it = this->ready_clients.begin(); it < this->ready_clients.end(); ++it)
    assert((*it) != id);
  // end debug.
  this->ready_clients.push_back(id);
}

unsigned long int Action::get_id() const
{
  return this->event->get_id();
}
