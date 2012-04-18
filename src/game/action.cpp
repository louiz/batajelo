#include <game/action.hpp>

Action::Action(t_action_callback callback):
  callback(callback)
{
}

Action::~Action()
{
}

void Action::execute() const
{
  this->callback();
}
