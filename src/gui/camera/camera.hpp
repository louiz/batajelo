/** @addtogroup Ui
 *  @{
 */

/**
 * The camera viewing the game. It keeps informations like zoom, position,
 * rotation, camera movements, etc. And with that and using a World object,
 * it displays the in-game image on the display.
 * @class Camera
 */

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#ifndef __CAMERA__HPP__
# define __CAMERA__HPP__

#include <logging/logging.hpp>
#include <world/client_world.hpp>
#include <world/time.hpp>
#include <gui/camera/map.hpp>

class Minimap;

class Camera
{
  friend class Minimap;
public:
  Camera(ClientWorld*, GraphMap*, sf::RenderWindow*);
  ~Camera();
  /**
   * Draw the world viewed through the camera.
   */
  void draw();
  /**
   * handle one user input event.
   */
  void handle_event(const sf::Event&);
  /**
   * Update the camera position or other stuff according to previous
   * input etc.
   */
  void update(const Duration& dt);

private:
  Camera(const Camera&);
  Camera& operator=(const Camera&);
  /**
   * Draw the given entity if it's visible by the camera.
   * Does nothing otherwise.
   */
  void draw_entity(const Entity*);
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
  uint movement_speed;
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
};

#endif // __CAMERA__HPP__
