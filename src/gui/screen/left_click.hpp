#ifndef LEFT_CLICK_HPP_INCLUDED
#define LEFT_CLICK_HPP_INCLUDED

#include <world/position.hpp>
#include <SFML/Graphics.hpp>

#include <functional>

#include <gui/cursor.hpp>

/**
 * Structure containing informations about the current “left click”
 * action. It contains a callback of what happens when the user left clicks
 * on the camera area, and what to draw at the cursor position.
 */
struct LeftClick
{
  /**
   * The callback called when we left click.  Returns True if the callback
   * has been successfull and the LeftClick can be reset to LeftClick::null.
   */
  std::function<bool(const Position& world_position)> callback;
  /**
   * The callback called when we draw the cursor (for example to draw the
   * area of effect of the selected spell, or the building to be built)
   */
  std::function<cursor::type (const sf::Vector2i& mouse_pos)> cursor_callback;
  static LeftClick null;
};

#endif /* LEFT_CLICK_HPP_INCLUDED */
