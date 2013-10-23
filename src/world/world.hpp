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

#ifndef __WORLD_HPP__
# define __WORLD_HPP__

#include <list>
#include <queue>
#include <functional>

#include <boost/archive/text_iarchive.hpp>

#include <game/event.hpp>
#include <world/occupant.hpp>
#include <world/entity.hpp>
#include <world/unit.hpp>
#include <world/time.hpp>
#include <world/map.hpp>
#include <game/action.hpp>
#include <game/event.hpp>
#include <game/turn_handler.hpp>
#include <game/replay.hpp>
#include <network/command.hpp>
#include <mpreal/mpreal.h>
#include <world/position.hpp>
#include <world/path.hpp>
#include <mod/mod.hpp>

/**
 * From left to right, and from top to bottom, a cell has this size, in the
 * world representation. This means that if you add 100 to an entity
 * position, it will move to the next cell.
 */
const unsigned int CELL_SIZE = 100;
#define LAYER_HEIGHT 32.f // MUST be 0.32 * CELL_SIZE

class World
{
public:
  World(Mod&);
  World(Map*, Mod&);
  ~World();
  /**
   * Init the world by reading the Mod files.
   * The initial state of the world (the map, the unit that are available
   * and their stats, etc).
   */
  void init(Mod&);
  /**
   * Start the world and it's turn_handler.
   * Before this method is called, tick() will do nothing.
   */
  void start();
  /**
   * Advance the world by the smallest step possible. Updates every entity,
   * timer, occupant, turn_handler etc it contains.
   * The bool tells us to actually do the tick even if the world is not
   * started. This is used when joining a game already started: we tick
   * as fast as possible to get to the current game situation, and then
   * we start the world normally to play the game at normal speed.
   */
  void tick(bool force = false);
  /**
   * returns a pointer to the next entity of the world. If it returns NULL,
   * this means that the list is over and the next call will restart from the
   * beginning of the list.
   * If the list is empty, it will always return NULL.
   */
  Entity* get_next_entity();
  /**
   * Just like the other get_next_entity(), but it will also return
   * NULL if the next entity's y is superior to the given y.
   */
  Entity* get_next_entity(const int y);
  /**
   * Insert a unit in the unit list. It will also be added into the entity list.
   */
  virtual void insert_unit(Unit*);
  virtual void insert_building(Building*);
  /**
   * Create an entity based on the given model.
   */
  Unit* create_unit(const unsigned int type);
  const Unit* get_unit_model(unsigned int type) const;
  /**
   * Create a building based on the given model.
   */
  Building* create_building(unsigned int type, const short x, const short y);
/**
   * Create an entity based on the given model, with the given
   * SerializableEntity to copy the initial entity position.
   */
  Unit* create_unit(unsigned int type, const Unit& e);
  void set_next_turn_callback(t_next_turn_callback);
  void pause();
  void unpause();
  void completely_validate_action(const unsigned int id);
  void validate_turn_completely(const unsigned int number);
  void generate_command(const char* name, const std::string& archive);
  Command* get_pending_command();
  /**
   * Call tick(true) until the turn_handler is paused.
   */
  void advance_replay_until_paused();
  /**
   * Actually instert a occupant in the occupants list.
   */
  void add_new_occupant(Occupant*);
  /**
   * Actually remove the occupant from the occupants list.
   */
  void remove_occupant(Occupant*);
  void do_path(ActionEvent*);
  Path calculate_path(Position, Unit*);
  void do_new_unit(ActionEvent*);
  void do_build(ActionEvent*);
  void do_spawn(ActionEvent*);
  Entity* get_entity_by_id(unsigned short id);
  Unit* get_unit_by_id(unsigned short id);
  Building* get_building_by_id(unsigned short id);
  bool can_unit_spawn_at_pos(const Unit*, const Position&) const;
  /**
   * Sends a command to the server saying that we confirm that action.
   */
  void confirm_action(const unsigned int);
  /**
   * Returns a pointer to the replay.
   */
  Replay* get_replay() const;

  TurnHandler* get_turn_handler() const;
  /**
   * Return the current number of connected clients.
   */
  unsigned int get_number_of_occupants() const;
  /**
   * Sort the entities by their y position. This
   * is used to draw them in the correct order.
   */
  void sort_entities();
  /**
   * Sets the coordinate of the cell containing the given world position.
   */
  void get_cell_at_position(const Position& pos,
                            short& xa, short& ya) const;
  /**
   * Returns the height (ingame) of the point at the given position.
   * It takes into account the highest level having a tile containing this
   * position and the tile height (see tile_heights in map.hpp)
   */
  mpreal get_position_height(const Position&) const;
  /**
   * Convert a path made of cells by a path composed of world positions
   */
  bool can_build_at_cell(const int x, const int y) const;
  Path smooth_path(cell_path_t path, Position& start,
                   const Position& end, const short width) const;
  /**
   * Returns wheither or not we can walk from the start position to the end
   * position, following a single straight line.
   */
  bool has_a_line_of_sight(const Position& start, const Position& end, const mpreal step, const short width) const;
  /**
   * Returns whether the entity can walk in a straight line from one point
   * to another.  This is done by checking if two parallel lines of sight
   * exist, separated by the width of the entity.
   */
  bool can_walk_in_straight_line(const Position& start, const Position& end, const mpreal step, const short width) const;
  /**
   * Returns whether or not we can move from one cell to another, according
   * to their respective heights.
   */
  bool can_traverse_cell(const short x, const short y,
                         const short x2, const short y2) const;
  /**
   * Return the position of the nearest corner of the given cell, using the next
   * cell to determine the optimal corner.
   */
  Position get_nearest_corner(const Position&, const std::size_t, const std::size_t, const short width) const;
  /**
  * Return the position of the nearest corner of the given cell, in the case
  * where that is the last cell of the path.
  */
  Position get_nearest_corner(const Position&, const std::size_t, const short width) const;
  Position get_next_path_position(cell_path_t& path, const Position current,
                                         const Position& end, const short width) const;
  std::size_t number_of_units_models() const
  {
    return this->unit_models.size();
  }
  bool is_started() const;
  /**
   * the list of other occupants of the game, when a new client connects to
   * the server, we add it to the list, when it disconnects we remove it.
   * Each occupant object contains an id corresponding to the
   * RemoteGameClient object stored in the server.
   */
  std::vector<Occupant*> occupants;
  /**
   * The list of all existing entities in the world.
   */
  std::list<Entity*> entities;
  /**
   * The list of all existing units in the world.
   */
  std::list<Unit*> units;
  /**
   * The list of all existing buildings in the world.
   */
  std::list<Building*> buildings;

private:
  /**
   * Insert an entity at the end of the list.
   */
  void insert_entity(Entity*);
  World(const World&);
  World& operator=(const World&);

protected:
  /**
   * The list of all models for the units in the world.
   * That list is created by loading a Mod file. To spawn a unit
   * in the world, we take the entity type in that list, and copy
   * it into a new unit. This list order is important, each
   * entity model is uniq, and that list does not change once the game
   * is started.
   */
  std::vector<const Unit*> unit_models;
  std::vector<const Building*> building_models;
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
  Replay* replay;
  Map* map;

public:
  // for debug
  cell_path_t current_path;
};

#endif // __WORLD_HPP__
/**@}*/
