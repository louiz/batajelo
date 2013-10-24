#include <game/turn_handler.hpp>
#include <logging/logging.hpp>

TurnHandler::TurnHandler(Replay* replay):
  current_turn(0),
  turn_advancement(0),
  paused(true),
  next_turn_callback(nullptr),
  replay(replay)
{
}

TurnHandler::~TurnHandler()
{
  this->turns.clear();
}

void TurnHandler::tick(bool force)
{
  if ((this->paused == true) && (force == false))
    {
      // we check if next turn is validated, to unpause.
      if (this->is_next_turn_validated() == false)
        return ;
      this->unpause();
      this->turn_advancement = 0;
      this->next_turn();
      return ;
    }
  this->turn_advancement += 1;
  if (this->turn_advancement == TURN_TIME)
    {
      if ((this->is_next_turn_validated() == false) && (force == false))
        {
          log_debug("Next turn is not validated");
          this->pause();
          return ;
        }
      else
        {
          this->turn_advancement = 0;
          this->next_turn();
        }
    }
}

void TurnHandler::next_turn()
{
  this->current_turn++;
  if (this->next_turn_callback != nullptr)
    this->next_turn_callback(this->current_turn);
  log_debug("current turn: " << this->current_turn);
  // if the deque is empty, no action has to be taken.
  if (this->turns.empty())
    return ;
  // remove the previous (now empty) turn.
  this->turns.pop_front();
  if (this->turns.empty())
    return ;
  log_warning("Turn number " << this->current_turn << ": ");
  Turn* turn = &(this->turns[0]);
  if (this->replay != nullptr)
    turn->execute(false);
  else
    turn->execute(true);
}

bool TurnHandler::insert_turn(const unsigned long turn)
{
  if (turn < this->current_turn)
    {
      log_error("Tried to insert an action in a turn already passed: " << turn << " <= " << this->current_turn);
      // Should not happen.
      assert(false);
      return false;
    }
  if (this->turns.size() < turn - this->current_turn + 1)
    {
      Turn dummy;
      log_debug("Resizing to " << turn - this->current_turn + 1);
      this->turns.resize(turn - this->current_turn + 1, dummy);
    }
  return true;
}

bool TurnHandler::insert_action(Action* action, const unsigned long turn)
{
  if (this->insert_turn(turn) == false)
    {
      delete action;
      return false ;
    }
  log_debug("Inserting action with id " << action->get_id() << " into turn " << turn);
  this->turns[turn - this->current_turn].insert(action);
  return true;
}

void TurnHandler::pause()
{
  assert(this->paused == false);
  log_warning("pausing turnhandler at turn " << this->current_turn);
  this->paused = true;
}

void TurnHandler::unpause()
{
  assert(this->paused == true);
  log_warning("unpausing turnhandler");
  this->paused = false;
}

bool TurnHandler::is_next_turn_validated() const
{
  if (this->turns.size() < 2)
    // There's no futur turn, it can't possibly be validated yet
    return false;
  return this->turns[1].is_validated();
}

Turn* TurnHandler::get_turn(const unsigned int number)
{
  if (this->turns.size() <= number - this->current_turn)
    return nullptr;
  return &this->turns[number - this->current_turn];
}

bool TurnHandler::validate_action(const unsigned int id, const unsigned long int by)
{
  std::deque<Turn>::iterator it;

  bool res;
  for (auto& turn: this->turns)
    for (auto& action: turn.get_actions())
      if (action->get_id() == id)
        {
          res = action->validate(by);
          if (res == true)
            this->replay->insert_action(action);
          return res;
        }
  log_warning("Action to validate was not found");
  // Action was not found, so it did'nt became completely validated
  return false;
}

void TurnHandler::completely_validate_action(const unsigned int id)
{
  std::deque<Turn>::iterator it;
  Action* action;
  for (auto& turn: this->turns)
    for (auto& action: turn.get_actions())
      if (action->get_id() == id)
        action->validate_completely();
}

bool TurnHandler::validate_turn(const unsigned int number,
                                const unsigned long int by,
                                const unsigned int confirmations_needed)
{
  if (this->insert_turn(number) == false)
    {
      return false;
    }
  Turn* turn = this->get_turn(number);
  if (turn == nullptr)
    return false;
  return turn->validate(by, confirmations_needed);
}

unsigned long TurnHandler::get_current_turn()
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

void TurnHandler::completely_validate_turn(const unsigned int number)
{
  this->insert_turn(number);
  Turn* turn = this->get_turn(number);
  assert(turn != nullptr);
  return turn->validate_completely();
}

bool TurnHandler::is_paused() const
{
  return this->paused;
}

std::ostream& operator<<(std::ostream& os, TurnHandler& turn_handler)
{
  os << "Turn Handler:" << std::endl;
  std::deque<Turn>::iterator it;
  unsigned long int turn = turn_handler.current_turn;
  for (it = turn_handler.turns.begin(); it != turn_handler.turns.end(); ++it)
    {
      os << "turn " << turn++ << "\t\t";
      os << (*it);
      os << std::endl;
    }
  return os;
}
