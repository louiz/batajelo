#include <game/turn_handler.hpp>

TurnHandler::TurnHandler():
  current_turn(0),
  turn_advancement(0),
  paused(true)
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
      if (this->is_next_turn_validated() == false)
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
  log_debug("current turn: " << this->current_turn);
  // if the deque is empty, no action has to be taken.
  if (this->turns.empty())
    return ;
  // remove the previous (now empty) turn.
  this->turns.pop_front();
  if (this->turns.empty())
    return ;
  log_debug("Turn number " << this->current_turn << ": ");
  this->turns[0].execute();
}

bool TurnHandler::insert_action(Action* action, const unsigned long turn)
{
  if (turn <= this->current_turn)
    {
      log_error("Tried to insert an action in a turn already passed: " << turn << " <= " << this->current_turn);
      delete action;
      return false;
    }
  if (this->turns.size() < turn - this->current_turn + 1)
    {
      Turn dummy;
      log_debug("Resizing to " << turn - this->current_turn + 1);
      this->turns.resize(turn - this->current_turn + 1, dummy);
    }
  log_debug("Inserting  into " << turn - this->current_turn);
  this->turns[turn - this->current_turn].insert(action);
  return true;
}

void TurnHandler::pause()
{
  assert(this->paused == false);
  log_debug("pausing turnhandler");
  this->paused = true;
}

void TurnHandler::unpause()
{
  assert(this->paused == true);
  log_debug("unpausing turnhandler");
  this->paused = false;
}

bool TurnHandler::is_next_turn_validated() const
{
  if (this->turns.size() < 2)
    // No action is in the next turn, so it's ok to go to it.
    return true;
  return this->turns[1].is_validated();
}

Turn* TurnHandler::get_turn(const unsigned int number)
{
  assert(number > this->current_turn);
  if (this->turns.size() <= number - this->current_turn)
    return 0;
  return &this->turns[number - this->current_turn];
}

bool TurnHandler::validate_action(const unsigned int id, const unsigned long int by)
{
  std::deque<Turn>::iterator it;
  Action* action;

  for (it = this->turns.begin(); it < this->turns.end(); ++it)
    {
      (*it).reset_action_iterator();
      while ((action = (*it).get_next_action()) != 0)
	{
	  if (action->get_id() == id)
	    {
	      action->validate(by);
	      return true;
	    }
	}
    }
  return false;
}

unsigned long TurnHandler::get_current_turn()
{
  return this->current_turn;
}
