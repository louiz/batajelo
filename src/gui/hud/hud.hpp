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

#include <gui/camera/camera.hpp>
#include <gui/screen/screen_element.hpp>
#include <gui/hud/info_hud.hpp>
#include <gui/hud/abilities_panel.hpp>

class Screen;
class GameClient;

class Hud: public ScreenElement
{
public:
  Hud(GameClient* game, Screen* screen);
  ~Hud();
  void draw() override final;
  bool handle_event(const sf::Event&) override final;
  void update(const utils::Duration& dt) override final;
  bool is_entity_hovered(const Entity*) const;
  void add_info_message(std::string&& text);
  void activate_ability(const std::size_t nb);
  bool handle_keypress(const sf::Event& event);
  bool handle_keyrelease(const sf::Event& event);

private:
  Hud(const Hud&);
  Hud& operator=(const Hud&);
  GameClient* game;

  sf::Texture hud_texture;
  sf::Sprite hud_sprite;
  sf::Font font;
  AbilitiesPanel abilities_panel;
  InfoHud info_hud;
};

#endif // __HUP_HPP__
