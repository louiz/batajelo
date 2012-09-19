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
#include <logging/logging.hpp>
#include <gui/cursor.hpp>

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
  bool is_entity_hovered(const Entity*) const;
  void set_cursor_type(const cursor::type);
  void draw_mouse_cursor();
  t_left_click_callback get_left_click_callback() const
  {
    return on_left_click;
  }
  void set_left_click_callback(const t_left_click_callback callback, const cursor::type cursor_type=cursor::Normal)
  {
    on_left_click = callback;
    this->set_cursor_type(cursor_type);
  }
  ClientWorld* get_world() const
  {
    return world;
  }

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
  sf::Sprite cursor_sprite;
  t_left_click_callback on_left_click;
};

#endif // __SCREEN_HPP__
