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
// #include <gui/hud/minimap.hpp>
// #include <gui/hud/selection_panel.hpp>
// #include <gui/hud/action_panel.hpp>
#include <mod/client_mod.hpp>
#include <gui/screen/screen_element.hpp>

class Screen;
class GameClient;

class Hud: public ScreenElement
{
public:
  Hud(GameClient* game, Screen* screen);
  ~Hud();
  void draw() override final;
  bool handle_event(const sf::Event&) override final;
  void update(const Duration& dt) override final;
  bool is_entity_hovered(const Entity*) const;
  void reset_left_click_action();

private:
  Hud(const Hud&);
  Hud& operator=(const Hud&);
  // Minimap minimap;
  // SelectionPanel selection_panel;
  // ActionPanel action_panel;
  GameClient* game;

  sf::Texture hud_texture;
  sf::Sprite hud_sprite;
};

#endif // __HUP_HPP__
