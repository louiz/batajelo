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

std::vector<Action*>& Replay::get_actions()
{
  return this->actions;
}
