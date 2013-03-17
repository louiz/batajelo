#include <logging/logging.hpp>
#include <game/replay.hpp>

Replay::Replay()
{
}

Replay::~Replay()
{
}

void Replay::insert_action(Action* action)
{
  log_warning("inserting action");
  this->actions.push_back(action);
}

void Replay::reset_action_iterator()
{
  this->actions_iterator = this->actions.begin();
}

Action* Replay::get_next_action()
{
  if (this->actions_iterator == this->actions.end())
    return nullptr;
  Action* action = *this->actions_iterator;
  ++this->actions_iterator;
  return action;
}
