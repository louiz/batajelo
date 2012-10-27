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

#ifndef __SCREEN_HPP__
# define __SCREEN_HPP__

#include <vector>

#include <SFML/Graphics.hpp>

#include <gui/camera/camera.hpp>
#include <gui/hud/hud.hpp>
#include <world/client_world.hpp>
#include <gui/cursor.hpp>
#include <mod/client_mod.hpp>

class Screen
{
public:
  Screen(ClientWorld*, GraphMap*, sf::RenderWindow*, ClientMod&);
  ~Screen();
  /**
   * Draw all elements of the screen.
   */
  void draw();
  void update(const Duration&);
  void handle_event(const sf::Event&);
  bool is_entity_hovered(const Entity*) const;
  void set_cursor_type(const cursor::type);
  void draw_mouse_cursor();
  t_left_click get_left_click() const
  {
    return on_left_click;
  }
  void set_left_click_callback(const t_left_click left_click);
  cursor::type draw_build_cursor(const unsigned int, const unsigned int y, const std::size_t);
  cursor::type draw_move_cursor(const uint, const uint, const std::size_t);
  ClientWorld* get_world() const
  {
    return world;
  }
  void reset_left_click_action();

private:
  Screen(const Screen&);
  Screen& operator=(const Screen&);
  sf::RenderWindow* win;
  Camera camera;
  ClientWorld* world;
  Hud hud;
  /**
   * The list of the cursors' images. We just draw them at the mouse
   * position. The texture is changed whenever needed (for example when
   * changing the on_left_click_action callback to a spell, we also set the
   * texture to the cursor::Cast one.
   */
  std::vector<sf::Texture> cursor_textures;
  cursor::type current_cursor_type;
  sf::Sprite cursor_sprite;
  t_left_click on_left_click;
  t_draw_cursor_action on_cursor_draw;
public:
  std::vector<const BuildingSprite*> building_sprites;
};

#endif // __SCREEN_HPP__
