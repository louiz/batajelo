/** @addtogroup Ui
 *  @{
 */

/**
 * Uniq object owned by the Screen. It is in charge of displaying all
 * static information about the game, like the time, the minimap, entities
 * attributes, interface buttons, etc.
 * @class Hud
 */

#include <SFML/System.hpp>

#ifndef __HUP_HPP__
# define __HUP_HPP__

#include <world/client_world.hpp>
#include <gui/hud/minimap.hpp>
#include <gui/camera/camera.hpp>

class Hud
{
public:
  Hud(GraphMap*, sf::RenderWindow*, Camera*);
  ~Hud();
  void draw(Camera*);
  bool handle_event(const sf::Event&);
private:
  Hud(const Hud&);
  Hud& operator=(const Hud&);
  Minimap minimap;
  sf::RenderWindow* win;
};

#endif // __HUP_HPP__
