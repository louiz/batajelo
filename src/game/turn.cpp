#include <logging/logging.hpp>
#include <game/turn.hpp>

#include <cassert>
Turn::Turn():
  ready(false)
{
}

Turn::~Turn()
{
}

void Turn::insert(Action&& action)
{
  assert(!this->ready);
  this->actions.push_back(std::move(action));
}

void Turn::execute()
{
  log_debug(this->actions.size() << " actions to execute");

  for (const auto& action: this->actions )
    {
      log_warning("Executing one action");
      action();
    }
}

void Turn::mark_ready()
{
  this->ready = true;
}

bool Turn::is_ready() const
{
  return this->ready;
}

const std::vector<Action>& Turn::get_actions() const
{
  return this->actions;
}
