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

#define HUD_HEIGHT 323

#include <world/client_world/client_world.hpp>
#include <gui/camera/camera.hpp>
#include <gui/hud/minimap.hpp>
#include <gui/hud/selection_panel.hpp>
#include <gui/hud/action_panel.hpp>
#include <mod/client_mod.hpp>

class Screen;

class Hud
{
public:
  Hud(GraphMap*, ClientWorld*, sf::RenderWindow*, Camera*, Screen*, ClientMod&);
  ~Hud();
  void draw(Camera*);
  bool handle_event(const sf::Event&);
  bool is_entity_hovered(const Entity*) const;
  void reset_left_click_action();

private:
  Hud(const Hud&);
  Hud& operator=(const Hud&);
  Minimap minimap;
  sf::RenderWindow* win;
  sf::Sprite hud_sprite;
  sf::Texture hud_texture;
  ClientWorld* world;
  SelectionPanel selection_panel;
  ActionPanel action_panel;
};

#endif // __HUP_HPP__
