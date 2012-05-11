#include <game/action.hpp>

Action::Action(actions::Type type):
  type(type)
{
}

Action::~Action()
{
}

void Action::execute() const
{
  // this->callback();
}
