/** @addtogroup Ui
 *  @{
 */

/**
 * The camera viewing the game. It keeps informations like zoom, position,
 * rotation, camera movements, etc. And with that and using a World object,
 * it displays the in-game image on the display.
 * @class Camera
 */

#ifndef __CAMERA__HPP__
# define __CAMERA__HPP__

#include <cstdlib>
#include <chrono>
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <utils/time.hpp>
#include <gui/camera/mouse_selection.hpp>
#include <fixmath/fix16.hpp>
#include <world/position.hpp>

#include <gui/screen/screen_element.hpp>
#include <gui/camera/tileset.hpp>
#include <gui/camera/fog.hpp>

#include <gui/sprites/energy_bar.hpp>

class Minimap;
class Screen;
class GameClient;
class Entity;
class EntitySprite;
class Task;

class Camera: public ScreenElement
{
  friend class Minimap;
  friend class Screen;
public:
  Camera(GameClient*, Screen*);
  ~Camera();
  const GameClient* get_game_client() const;
  /**
   * Draw the world viewed through the camera.
   */
  void draw();
  /**
   * Draw the rectangle representing the mouse selection.
   */
  void draw_mouse_selection();
  /**
   * Draw an energy bar, centered at the given screen position.
   */
  void draw_energy_bar(const EnergyBar& bar, const std::size_t max_val,
                       int current_val, const sf::RenderStates& states);
  void draw_vertical_bar(sf::Vector2f screen_position, const EnergyBar& bar_specs,
                         const std::size_t max_val, int current_val);
  bool is_mouse_over(const EntitySprite* sprite) const;
  /**
   * Draw an indicator, centered around `center`, the width is the diameter.
   */
  void draw_hover_indicator(const sf::Vector2i& center, const unsigned int width);
  void draw_selected_indicator(const sf::Vector2i& center, const unsigned int width);

  /**
   * Try to activate some ability with one of the selected entity. If no
   * entity is selected, do nothing, if one or more are selected, find one
   * that can cast it.
   *
   * Note that activating may only be the first step, and may not cast
   * anything. For example, if the ability needs to be given a target, we
   * only set the LeftClick value, and wait the user to actually select a
   * target before casting the spell.
   */
  void activate_ability(const std::size_t nb);

  /**
   * handle one user input event.
   */
  bool handle_event(const sf::Event&);
  void handle_right_click(const sf::Event&);
  void handle_middle_click(const sf::Event&);
  void handle_left_click(const sf::Event&);
  void handle_left_release(const sf::Event&);
  void handle_middle_release(const sf::Event&);
  void handle_keypress(const sf::Event&);
  /**
   * Update the camera position or other stuff according to previous
   * input etc.
   */
  void update(const utils::Duration& dt);
  /**
   * Center the camera on the given screen position
   */
  void center(const sf::Vector2i& center);
  sf::Vector2i world_to_camera_position(const Position&) const;
  sf::Vector2i world_to_screen_position(const Position&) const;
  /**
   * Convert a mouse position, screen-related, into a world position
   */
  Position screen_to_world_position(const int, const int) const;
  /**
   * Like screen_to_world_position, but with a camera-related position of
   * the mouse
   */
  Position camera_to_world_position(const int, const int) const;
  /**
   * Return the mouse position, taking into account the position of the
   * camera. This means that if the camera has an offset of 100px (x) and
   * 100px (y), the result is {100, 100} if the mouse is in the top left
   * corner of the window.
   *
   * Do not confuse with Screen::get_mouse_position()
   */
  sf::Vector2i get_mouse_position() const;
  /**
   * Return the cell at the given position. If the position is not contained
   * by any cell, return UINT_MAX.
   */
  CellIndex get_cell_at_coord(const unsigned int x, const unsigned int y) const;
  /**
   * Start a mouse selection, i.e. drawing a rectangle to select entities
   * inside it. The position is a world-static position, not a camera
   * position; this means that if the camera moves while there's an ongoing
   * mouse selection, the start position of the mouse selection may go out
   * of the camera's sight.
   */
  void start_mouse_selection(const sf::Vector2u& pos);
  /**
   * Takes everything under mouse selection and add it to the selection.
   */
  void add_mouse_selection_to_selection();
  /**
   * Takes everything under mouse selection and set it as the selection.
   * Replaces the previous selection completely.
   */
  void set_mouse_selection_to_selection();
  /**
   * Returns whether or not a mouse selection (by dragging) is currently
   * ongoing or not.
   */
  bool is_mouse_selection_ongoing() const;
  /**
   * Return a pointer to a selectable entity under the mouse, nullptr if there is none
   */
  const Entity* get_entity_under_mouse() const;

  void draw(const sf::Drawable&, const sf::RenderStates& states = sf::RenderStates::Default);
  void on_new_entity(const Entity*);
  void on_entity_deleted(const Entity*);
  void on_entity_task_changed(const Entity* entity, const Task* task);
  void graphical_tick();

  const sf::Vector2u get_win_size() const;
  /**
   * The left position of the camera
   */
  double x;
  /**
   * The top position of the camera
   */
  double y;
  /**
   * The zoom factor. A negative value would display the game
   * flipped over.
   */
  double zoom;

private:
  Camera(const Camera&);
  Camera& operator=(const Camera&);

  /**
   * Check if the camera is at a valid position. If not, fix it.
   */
  void fixup_camera_position();
  /**
   * Weither or not the window has the focus.
   */
  bool focused;
  /**
   * The speed at which the camera moves.
   */
  float movement_speed;
  /**
   * The position of the cursor. It is used to know the movement the camera
   * has to do, when middle click is pressed and the mouse is moved.
   */
  sf::Vector2i previous_position;
  /**
   * The position of the cursor when the user started dragging the camera.
   */
  sf::Vector2i start_drag_position;
  /**
   * A pointer to the world object used to display stuff on the screen.
   */
  GameClient* game;
  std::list<std::unique_ptr<EntitySprite>> sprites;
  MouseSelection mouse_selection;
  Tileset tileset;
  Fog fog;

  /**
   * Various accessors
   */
  World& world() const;
  Map& map() const;
  sf::RenderWindow& win();
  sf::RenderWindow& win() const;
};

 // debug
static CellIndex selected_cell;

/**
 * Given a relative (to the cell) x and two corners heights, returns the
 * height of the cell at the x position
 */
float get_edge_height();

#endif // __CAMERA__HPP__
