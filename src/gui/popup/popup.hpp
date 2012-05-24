/** @addtogroup Ui
 *  @{
 */

/**
 * Popup
 * @class Popup
 */

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#ifndef __POPUP_HPP__
# define __POPUP_HPP__

class Ui;

class Popup
{
public:
  Popup(Ui*, sfg::Desktop*, sf::RenderWindow*);
  virtual void hide() = 0;
  /**
   * Draw the background.
   * @return void
   */
  void draw_background();
  /**
   * Callback on resize event.
   * @return void
   */
  void on_resize();

  sfg::Window::Ptr window_popup;
  sfg::Box::Ptr box_popup;
  sfg::Desktop* desktop;
  sf::RenderWindow* window;
  sf::RectangleShape bg;
  Ui* ui;
private:
   virtual void build_ui() = 0;
  /**
   * Build the background.
   * @return void
   */
  void build_background();
};

#endif // __POPUP_HPP__
/**@}*/