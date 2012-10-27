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
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <boost/function.hpp>

#include <world/client_world.hpp>
#include <world/time.hpp>
#include <gui/camera/map.hpp>
#include <gui/camera/mouse_selection.hpp>
#include <mpreal/mpreal.h>
#include <world/position.hpp>

class Minimap;
class Screen;

class Camera
{
  friend class Minimap;
  friend class Screen;
public:
  Camera(ClientWorld*, GraphMap*, sf::RenderWindow*, Screen*);
  ~Camera();
  /**
   * Draw the world viewed through the camera.
   */
  void draw();
  /**
   * Draw the rectangle representing the mouse selection.
   */
  void draw_mouse_selection();
  /**
   * Draw the unit on the screen, taking the on-screen position of the
   * unit.
   */
  void draw_unit(const Unit*, const uint x, const uint y,
                   const bool in_mouse_selection, sf::RectangleShape& rectangle);
  /**
   * handle one user input event.
   */
  bool handle_event(const sf::Event&);
  void handle_right_click(const sf::Event&);
  void handle_middle_click(const sf::Event&);
  void handle_left_click(const sf::Event&);
  void handle_left_release(const sf::Event&);
  void handle_middle_release(const sf::Event&);
  /**
   * Update the camera position or other stuff according to previous
   * input etc.
   */
  void update(const Duration& dt);
  sf::Vector2u world_to_camera_position(const Position&) const;
  Position camera_to_world_position(const int, const int) const;
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

private:
  Camera(const Camera&);
  Camera& operator=(const Camera&);
  /**
   * Check if the camera is at a valid position. If not, fix it.
   */
  void fixup_camera_position();
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
  ClientWorld* world;
  GraphMap* map;
  sf::RenderWindow* win;
  MouseSelection mouse_selection;
  Screen* screen;
};

#endif // __CAMERA__HPP__
