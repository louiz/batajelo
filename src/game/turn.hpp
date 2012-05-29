#ifndef __TURN_HPP__
# define __TURN_HPP__

#include <game/action.hpp>

class Turn
{
public:
  friend std::ostream& operator<<(std::ostream& os, Turn& turn);
  Turn();
  ~Turn();
  void execute(bool delete_actions = true);
  void insert(Action*);
  bool is_validated() const;
  Action* get_next_action();
  void reset_action_iterator();
  /**
   * Validate the turn for the given client id.
   * If the number of ready clients becomes equal to the given number needed,
   * validate_completely() is called and returns true. Returns false otherwise
   */
  bool validate(const unsigned long int by,
		const unsigned int confirmations_needed);
  /**
   * Return the number of validations for this turn, so far.
   */
  unsigned int get_number_of_validations() const;
  /**
   * Just set the validated bool to true.
   */
  void validate_completely();

private:
  std::vector<Action*> actions;
  std::vector<Action*>::const_iterator actions_iterator;

  std::vector<unsigned long int> ready_clients;
  bool validated;
};


#endif // __TURN_HPP__
