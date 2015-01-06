/** @addtogroup Ui
 *  @{
 */

/**
 * A simple class defining a start position and some methods to know if an
 * entity is inside the mouse selection or not.
 * @class MouseSelection
 */

#ifndef __MOUSE_SELECTION_HPP__
# define __MOUSE_SELECTION_HPP__

#include <assert.h>
#include <SFML/Graphics.hpp>

class Camera;

class MouseSelection
{
  friend class Camera;
public:
  MouseSelection();
  ~MouseSelection() = default;
  void start(const sf::Vector2i&);
  void end();
  /**
   * Returns whether or not the mouse selection contains that point.
   */
  bool contains(const sf::Vector2i&,
                const sf::Vector2i&, const uint width = 1) const;

private:
  MouseSelection(const MouseSelection&);
  MouseSelection& operator=(const MouseSelection&);
  /**
   * The position at which the mouse was pressed.
   */
  sf::Vector2i start_pos;
  bool ongoing;
};

#endif // __MOUSE_SELECTION_HPP__

