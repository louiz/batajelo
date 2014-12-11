/**
 * Main class inheriting from BaseClient, and linking the game classes
 * together: World, Camera and all GUI things.
 */

#ifndef GAME_CLIENT_HPP_INCLUDED
#define GAME_CLIENT_HPP_INCLUDED

#include <game/game.hpp>
#include <game/occupants_handler.hpp>
#include <network/client_base.hpp>
#include <gui/screen/screen.hpp>
#include <game/selection.hpp>
#include <game/turn_handler.hpp>
#include <world/world.hpp>
#include <gui/hud/debug_hud.hpp>
#include <gui/hud/hud.hpp>

#include <memory>

class GameClient: public Game, public ClientBase
{
public:
  explicit GameClient(const std::shared_ptr<Screen>& screen);
  ~GameClient();
  void run();
  void install_callbacks() override final;
  void on_connection_closed() override final;

  void add_new_occupant(std::unique_ptr<Occupant>&& occupant);

  void do_new_unit(const EntityType type, const Position& pos, const uint16_t team) override final;

  void send_message(const char* name, const google::protobuf::Message& msg);
  void send_message(const char* name, const std::string& archive);

  World& get_world();
  Camera& get_camera();
  Screen& get_screen();

  DebugHud& get_debug_hud();

  /**
   * Called whenever we receive a occupant_left message from the server.
   */
  void occupant_left_callback(Message*);
  /**
   * Called when a new unit has to be inserted in the world.
   */
  // void new_unit_callback(Message*);
  /**
   * When we receive the message from the server telling us that
   * it is ready to start the game.
   */
  void handle_start_message(Message*);
  /**
   * The server tells us that this turn is ready to be executed (the server
   * already executed it, and thus no more action will be added to it)
   */
  void turn_callback(Message*);
  /**
   * The server gives a path to follow, to one or more entities.
   */
  void path_callback(Message*);
  /**
   * The server tells one unit to build the given building
   */
  void build_callback(Message*);
  /**
   * The server tells one building to spawn the given unit
   */
  void spawn_callback(Message*);
  /**
   * Insert an action, built from a message we received, into the turn handler.
   */
  // void insert_received_action(std::unique_ptr<Action>&&, ActionEvent* event);
  /**
   * Insert a sprite for the unit, as well, and then call world->insert_unit()
   */
  void on_new_building(Building*);

  /**
   * Handle an user event that may generate a real action, or nothing.
   * The coordinates are the real-world ones.
   * (for example if the user gives on order while no unit is selected, nothing
   * happens).
   * If an action is generated with this event, generate_action() is called.
   */
  // void handle_event(actions::Type type, unsigned int x, unsigned y);

  /**
   * Tick the TurnHandler and the World, if the TurnHandler is not paused.
   */
  void tick();
  /**
   * Update all graphical object states (tiles etc)
   */
  void graphical_tick();

  /**
   * Give the order to all selected and movable units to move to the given
   * world coordinates.
   */
  bool action_move(std::vector<EntityId> ids, const Position& pos,
                   const bool queue);
  // bool action_move(const unsigned int x, const unsigned y, const std::size_t=0);
  bool action_build(const unsigned int, const unsigned int, const std::size_t);
  void action_spawn(const t_left_click left_click);
  /**
   * Give the order to all selected and movable units to move to the given
   * world coordinates with an attack order (will attack all encountered
   * enemy entity in range) OR to attack the entity under the mouse if any.
   */
  void action_attack(const unsigned int x, const unsigned y);
  void select_entity(const Entity*);
  void unselect_entity(const Entity*);
  void clear_selection();
  bool is_entity_selected(const Entity*) const;
  const Selection& get_selection() const;
  const Selection* get_selection_ptr() const
  {
    return &this->current_selection;
  }
  void add_selection_change_callback(const t_selection_changed_callback);
  void set_self_team(const uint16_t team);

private:
  /**
   * Called by the world object when a turn ends and a new one starts
   */
  void on_next_turn(TurnNb turn);
  /**
   * Called whenever we receive a new_occupant message from the server.
   */
  void new_occupant_callback(Message*);

  Selection current_selection;
  std::shared_ptr<Screen> screen;
  Camera camera;
  Hud hud;
  DebugHud debug_hud;
  uint16_t team;

  GameClient(const GameClient&) = delete;
  GameClient(GameClient&&) = delete;
  GameClient& operator=(const GameClient&) = delete;
  GameClient& operator=(GameClient&&) = delete;
};

#endif /* GAME_CLIENT_HPP_INCLUDED */
