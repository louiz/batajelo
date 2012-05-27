#ifndef __CLIENT_WORLD_HPP__
# define __CLIENT_WORLD_HPP__

#include <world/world.hpp>

class ClientWorld: public World
{
public:
  ClientWorld();
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
  void on_next_turn(unsigned long turn);

private:
  ClientWorld(const ClientWorld&);
  ClientWorld& operator=(const ClientWorld&);
};

#endif // __CLIENT_WORLD_HPP__
