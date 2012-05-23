/**
 * @defgroup World
 * This group contains all elements representing the world or contained in it.
 * Units, building, etc. It is shared by the clients and game server and should
 * be 100% deterministic (no random, no float at all, etc), to be fully
 * synchronized everywhere.
 */

/** @addtogroup World
 *  @{
 */

/**
 * The world, contains the list of all entities, the game time, etc.
 */

#include <list>
#include <queue>

#ifndef __WORLD_HPP__
# define __WORLD_HPP__

#include <world/occupant.hpp>
#include <world/entity.hpp>
#include <world/time.hpp>
#include <game/action.hpp>
#include <game/event.hpp>
#include <game/turn_handler.hpp>
#include <network/command.hpp>

class World
{
public:
  World();
  ~World();
  /**
   * Init the world by reading the Mod files.
   */
  void init();
  /**
   * Start the world and it's turn_handler.
   * Before this method is called, tick() will do nothing.
   */
  void start();
  /**
   * Advance the world by the smallest step possible. Updates every entity,
   * timer, occupant, turn_handler etc it contains.
   */
  void tick();
  /**
   * returns a pointer to the next entity of the world. If it returns NULL,
   * this means that the list is over and the next call will restart from the
   * beginning of the list.
   * If the list is empty, it will always return NULL.
   */
  Entity* get_next_entity();
  /**
   * Set the entity iterator at its beginning.
   */
  void reset_entity_iterator();
  /**
   * Insert an entity at the end of the list.
   */
  void insert_entity(Entity*);
  /**
   * Handle an user event that may generate a real action, or nothing.
   * The coordinates are the real-world ones.
   * (for example if the user gives on order while no unit is selected, nothing
   * happens).
   * If an action is generated with this event, generate_action() is called.
   */
  void handle_event(actions::Type type, unsigned int x, unsigned y);
  /**
   * Create an entity based on the given model.
   */
  Entity* create_entity(unsigned int type);
  void generate_command(const char* name, const std::string& archive);
  Command* get_pending_command();

  void set_next_turn_callback(t_next_turn_callback);
  void on_next_turn(unsigned long turn);

  void pause();
  void unpause();

  void load_test();

  void install_start_action(Event*, unsigned int);

  bool validate_action(const unsigned int id, const unsigned long int by);
  void completely_validate_action(const unsigned int id);
  bool validate_turn(const unsigned int id, const unsigned long int by);
  void validate_turn_completely(const unsigned int number);

  void confirm_action(const unsigned long int id);
  /**
   * Called whenever we receive a new_occupant message from the server.
   */
  void new_occupant_callback(Command*);
  /**
   * Actually instert a occupant in the occupants list.
   */
  void add_new_occupant(Occupant*);
  /**
   * Called whenever we receive a occupant_left message from the server.
   */
  void occupant_left_callback(Command*);
  /**
   * Actually remove the occupant from the occupants list.
   */
  void remove_occupant(Occupant*);
  /**
   * Called when a new unit has to be inserted in the world.
   */
  void new_entity_callback(Command*);

  /**
   * When we receive the command from the server telling us that
   * it is ready to start the game.
   */
  void handle_start_command(Command*);

  void confirm_initial_turn();

  void move_callback(Command*);

  void ok_callback(Command*);
  void turn_callback(Command*);
  void path_callback(Command*);
  void do_path(Event*);
  Entity* get_entity_by_id(unsigned short id);
  /**
   * Sends a command to the server saying that we are ready for that turn.
   */
  void confirm_turn(const unsigned int);
  /**
   * Sends a command to the server saying that we confirm that action.
   */
  void confirm_action(const unsigned int);
  /**
   * the list of other occupants of the game, when a new client connects to
   * the server, we add it to the list, when it disconnects we remove it.
   * Each occupant object contains an id corresponding to the
   * RemoteGameClient object stored in the server.
   */
  std::vector<Occupant*> occupants;

private:
  World(const World&);
  World& operator=(const World&);
  /**
   * The list of all models for the entities in the world.
   * That list is created by loading a Mod file. To spawn a unit
   * in the world, we take the entity type in that list, and copy
   * it into a new unit. This list order is important, each
   * entity model is uniq, and that list does not change once the game
   * is started.
   */
  std::vector<Entity*> entity_models;
  /**
   * The list of all existing entities in the world.
   */
  std::list<Entity*> entities;
  /**
   * An iterator to help other classes get the entities one by
   * one without having to play with an iterator themselve, etc.
   */
  std::list<Entity*>::iterator entities_iterator;
  /**
   * The list of all action generated that needs to be sent to the
   * the remote server or the clients.
   * The server may generate actions too, in case of a game versus an IA or
   * something like that.
   */
  std::queue<Command*> commands_queue;
  /**
   *
   */
  TurnHandler* turn_handler;
  bool started;
};

#endif // __WORLD_HPP__
/**@}*/
