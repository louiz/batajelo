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

class Camera
{
public:
  Camera(ClientWorld*);
  ~Camera();
  /**
   * Draw the world viewed through the camera.
   */
  void draw(sf::RenderWindow*);
  /**
   * Draw the given entity if it's visible by the camera.
   * Does nothing otherwise.
   */
  void draw_entity(sf::RenderWindow* win, const Entity*);
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
   * The left position of the camera
   */
  unsigned int x;
  /**
   * The top position of the camera
   */
  unsigned int y;
  /**
   * The zoom factor. A negative value would display the game
   * flipped over.
   */
  unsigned int zoom;
  /**
   * A pointer to the world object used to display stuff on the screen.
   */
  ClientWorld* world;
};

#endif // __CAMERA__HPP__
