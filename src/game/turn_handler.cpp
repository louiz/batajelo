#include <game/turn_handler.hpp>
#include <logging/logging.hpp>

#include <algorithm>
#include <cassert>

TurnHandler::TurnHandler():
  current_turn(0),
  turn_advancement(0),
  paused(true),
  next_turn_callback(nullptr)
{
}

TurnHandler::~TurnHandler()
{
  this->turns.clear();
}

void TurnHandler::tick()
{
  if (this->paused == true)
    {
      // we check if next turn is validated, to unpause.
      if (this->is_next_turn_ready() == false)
        return ;
      this->unpause();
      this->next_turn();
      return ;
    }
  this->turn_advancement++;
  if (this->turn_advancement == TURN_TICKS)
    {
      if (this->is_next_turn_ready() == false)
        {
          this->pause();
          return ;
        }
      else
        this->next_turn();
    }
}

void TurnHandler::next_turn()
{
  this->current_turn++;
  this->turn_advancement = 0;

  if (this->next_turn_callback)
    this->next_turn_callback(this->current_turn);

  log_warning("Advancing to turn number " << this->current_turn << ": ");

  this->turns.front().execute();
  this->turns.pop_front();
}

void TurnHandler::insert_turn(const TurnNb nb)
{
  if (nb <= this->current_turn)
    {
      log_error("Tried to insert an action in a turn already passed: " << nb << " <= " << this->current_turn);
      assert(false);
    }
  if (this->turns.size() < nb - this->current_turn)
    {
      log_debug("Resizing to " << nb - this->current_turn);
      this->turns.resize(nb - this->current_turn);
    }
}

void TurnHandler::insert_action(Action&& action, const TurnNb nb)
{
  // Make sure the turn into which the action must be inserted exist.
  this->insert_turn(nb);

  log_debug("Inserting action into turn " << nb);
  this->turns[nb - this->current_turn - 1].insert(std::move(action));
}

void TurnHandler::pause()
{
  log_warning("pausing turnhandler at turn " << this->current_turn);
  this->paused = true;
}

void TurnHandler::unpause()
{
  log_warning("unpausing turnhandler");
  this->paused = false;
}

bool TurnHandler::is_next_turn_ready() const
{
  if (this->turns.empty())
    // There's no futur turn, it can't possibly be ready yet
    return false;
  return this->turns[0].is_ready();
}

unsigned long TurnHandler::get_current_turn() const
{
  return this->current_turn;
}

std::deque<Turn>& TurnHandler::get_turns()
{
  return this->turns;
}

void TurnHandler::set_next_turn_callback(t_next_turn_callback callback)
{
  this->next_turn_callback = callback;
}

bool TurnHandler::is_paused() const
{
  return this->paused;
}

void TurnHandler::mark_turn_as_ready()
{
  auto it = std::find_if_not(this->turns.begin(), this->turns.end(),
                         [](const auto& turn)
                         {
                           return turn.is_ready();
                         });
  if (it == this->turns.end())
    it = this->turns.emplace(this->turns.end());
  it->mark_ready();
  this->last_ready_turn++;
}

void TurnHandler::mark_as_ready_until(const TurnNb n)
{
  log_debug("mark_as_ready_until(" << n << ")");
  this->insert_turn(n);
  this->last_ready_turn = n;
  std::size_t t = this->current_turn;

  for (auto& turn: this->turns)
    {
      turn.mark_ready();
      if (++t == n)
        return;
    }
}

TurnNb TurnHandler::get_last_ready_turn() const
{
  return this->last_ready_turn;
}
