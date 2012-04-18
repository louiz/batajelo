#include <game/turn_handler.hpp>

TurnHandler::TurnHandler():
  current_turn(0),
  turn_advancement(0)
{
}

TurnHandler::~TurnHandler()
{
  std::deque<std::vector<const Action*> >::iterator turn_it;
  std::vector<const Action*>::iterator action_it;
  for (turn_it = this->turns.begin(); turn_it < this->turns.end(); ++turn_it)
    {
      for (action_it = (*turn_it).begin(); action_it < (*turn_it).end(); ++action_it)
	{
	  delete (*action_it);
	}
    }
}

void TurnHandler::tick()
{
  this->turn_advancement += 1;
  if (this->turn_advancement == TURN_TIME)
    {
      this->turn_advancement = 0;
      this->next_turn();
    }
}

void TurnHandler::next_turn()
{
  this->current_turn++;
  log_debug("current turn: " << this->current_turn);
  // if the deque is empty, no action has to be taken.
  if (this->turns.empty())
    return ;
  // remove the empty vector of the previous turn
  this->turns.pop_front();
  // we execute the actions one by one, then delete them and remove them from the turn.
  std::vector<const Action*>::iterator it;
  log_debug("Turn number " << this->current_turn << ": " << this->turns[0].size() << " actions to execute");
  for (it = this->turns[0].begin(); it < this->turns[0].end(); ++it)
    {
      (*it)->execute();
      delete (*it);
    }
  this->turns[0].clear();
}

bool TurnHandler::insert_action(const Action* action, unsigned long turn)
{
  if (turn <= this->current_turn)
    {
      log_error("Tried to insert an action in a turn already passed: " << turn << " <= " << this->current_turn);
      delete action;
      return false;
    }
  if (this->turns.size() < turn - this->current_turn)
    {
      this->turns.resize(turn - this->current_turn + 1);
    }
  this->turns[turn - this->current_turn].push_back(action);
  return true;
}
