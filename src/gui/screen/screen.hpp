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

namespace cursor
{
  enum type
    {
      Normal,
      Select,
      Aim,
      Cast,
      // Just keep track of the possible number of cursors
      size
    };

  static const char filenames[size][15] =
  {
    "gui_cursor.png",
    "gui_cursor.png",
    "gui_cursor.png",
    "gui_cursor.png"
  };
}

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

private:
  Screen(const Screen&);
  Screen& operator=(const Screen&);
  sf::RenderWindow* win;
  Camera camera;
  Hud hud;

  /**
   * The list of the cursors' images. We just draw them at the mouse
   * position. The texture is changed whenever needed (for example when
   * changing the on_left_click_action callback to a spell, we also set the
   * texture to the cursor::Cast one.
   */
  std::vector<sf::Texture> cursor_textures;
  sf::Sprite cursor_sprite;
};

#endif // __SCREEN_HPP__
