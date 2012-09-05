#ifndef __CLIENT_WORLD_HPP__
# define __CLIENT_WORLD_HPP__

#include <world/world.hpp>
#include <world/selection.hpp>

class ClientWorld: public World
{
public:
  ClientWorld();
  ClientWorld(Map*);
  ~ClientWorld();
  /**
   * Tells the server that we received and inserted the action into
   * our turn_handler, so we are ready to execute it when its turn comes.
   */
  void confirm_action(const unsigned long int id);
  /**
   * Sends a command to the server saying that we are ready for that turn.
   */
  void confirm_turn(const unsigned int);
  /**
   * Confirm the turn to which a START command was associated.
   * No turn before this one should have been confirmed, because
   * they were part of the replay, so the server doesn't want any
   * confirmation for them.
   */
  void confirm_initial_turn();
  /**
   * Called whenever we receive a new_occupant message from the server.
   */
  void new_occupant_callback(Command*);
  /**
   * Called whenever we receive a occupant_left message from the server.
   */
  void occupant_left_callback(Command*);
  /**
   * Called when a new unit has to be inserted in the world.
   */
  void new_entity_callback(Command*);
  /**
   * When we receive the command from the server telling us that
   * it is ready to start the game.
   */
  void handle_start_command(Command*);
  /**
   * The server tells us that this action has been confirmed by all
   * clients.
   */
  void ok_callback(Command*);
  /**
   * The server tells us that this turn has been confirmed by all clients.
   */
  void turn_callback(Command*);
  /**
   * The server gives a path to follow, to one or more entities.
   */
  void path_callback(Command*);
  /**
   * Insert an action, built from a command we received, into the turn handler,
   * and confirm it or validate it, depending on if the game is started or not
   * (meaning that it comes from the server replay).
   */
  void insert_received_action(Action* action, ActionEvent* event);

  void on_next_turn(unsigned long turn);
  /**
   * Handle an user event that may generate a real action, or nothing.
   * The coordinates are the real-world ones.
   * (for example if the user gives on order while no unit is selected, nothing
   * happens).
   * If an action is generated with this event, generate_action() is called.
   */
  void handle_event(actions::Type type, unsigned int x, unsigned y);
  void select_entity(const Entity*);
  void unselect_entity(const Entity*);
  void clear_selection();
  bool is_entity_selected(const Entity*) const;
  const Selection& get_selection() const;
  const Selection* get_selection_ptr() const
  {
    return &this->current_selection;
  }
private:
  ClientWorld(const ClientWorld&);
  ClientWorld& operator=(const ClientWorld&);
  Selection current_selection;
};

#endif // __CLIENT_WORLD_HPP__
