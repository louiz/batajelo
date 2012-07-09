/** @addtogroup Ui
 *  @{
 */

/**
 * Screen contains all classes responsible for drawing the game on
 * the display. For example Camera draws the world (the map, all entities etc),
 * Hud displays all informational static (in position) elements (the minimap,
 * the entities informations, ressources, etc).
 * @class Screen
 */

#include <SFML/Graphics.hpp>

#ifndef __SCREEN_HPP__
# define __SCREEN_HPP__

#include <gui/camera/camera.hpp>
#include <gui/hud/hud.hpp>
#include <world/client_world.hpp>
#include <logging/logging.hpp>

class Screen
{
public:
  Screen(ClientWorld*, GraphMap*, sf::RenderWindow*);
  ~Screen();
  /**
   * Draw all elements of the screen.
   */
  void draw();
  void update(const Duration&);
  void handle_event(const sf::Event&);

private:
  Screen(const Screen&);
  Screen& operator=(const Screen&);
  sf::RenderWindow* win;
  Camera camera;
  Hud hud;
};

#endif // __SCREEN_HPP__
