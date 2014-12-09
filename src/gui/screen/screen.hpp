/** @addtogroup Ui
 *  @{
 */

/**
 * Contains a list of elements that can be displayed on the screen and that
 * interact with the user input. Examples of such elements include the
 * Camera (displaying the actual game, sprites, effects, tiles, etc), the
 * game HUD, various menu, etc.
 *
 * These elements are sorted by priority, which basically represents its
 * depth on the screen.  Elements with a lower priority are drawn behind the
 * ones with a higher priority.
 *
 * These objects are ScreenElement instances. Every event (mouse click, key
 * press, etc) is passed to these elements one by one (from the highest
 * priority to the lowest priority).  An element with a higher priority can
 * decide to “eat” the event, and lower priority events will not receive it.
 *
 * These ScreenElement objects are not owned by the Screen.  We only have a
 * list of weak_ptr, when a pointer becomes invalid, we remove it from the
 * list.
 *
 */

#ifndef __SCREEN_HPP__
# define __SCREEN_HPP__

#include <gui/hud/action_panel_button.hpp>
#include <gui/screen/screen_element.hpp>
#include <gui/cursor.hpp>
#include <game/time.hpp>

#include <vector>
#include <memory>

class Screen
{
public:
  Screen();
  ~Screen();
  /**
   * Draw all elements of the screen.
   */
  void draw();
  /**
   * Update all elements
   */
  void update(const Duration&);
  /**
   * Pass the event to all elements (until eaten)
   */
  void handle_event(const sf::Event&);
  /**
   * Add an element that should be updated, drawn etc
   */
  void add_element(ScreenElement* element, const int priority);
  // bool is_entity_hovered(const Entity*) const;
  void set_cursor_type(const cursor::type);
  void draw_mouse_cursor();
  t_left_click get_left_click() const
  {
    return on_left_click;
  }
  void set_left_click_callback(const t_left_click left_click);
  cursor::type draw_build_cursor(const unsigned int, const unsigned int y, const std::size_t);
  cursor::type draw_move_cursor(const uint, const uint, const std::size_t);
  void reset_left_click_action();

  sf::RenderWindow& window();
  sf::Vector2i get_mouse_position() const;
  const sf::Vector2u get_window_size() const;

private:
  Screen(const Screen&);
  Screen& operator=(const Screen&);
  sf::RenderWindow win;
  /**
   * A list of elements, sorted by priority, that should be updated, drawn
   * and to which user events are passed.
   */
  std::vector<ScreenElement*> screen_elements;

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
  std::vector<std::unique_ptr<sf::Texture>> building_textures;
};

#endif // __SCREEN_HPP__
