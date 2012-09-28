/** @addtogroup Ui
 *  @{
 */

/**
 * Home
 * @class Home
 */

#include <SFGUI/SFGUI.hpp>

#ifndef __HOME_HPP__
# define __HOME_HPP__

#include <gui/page/page.hpp>

class Home: public Page
{
public:
  Home(Ui*, sfg::Desktop*, sf::RenderWindow*);
  virtual ~Home() {}
  
  /**
   * Hide the page.
   * @return void
   */
  virtual void hide();
  /**
   * Show the page.
   * @return void
   */
  virtual void show();
  /**
   * Activate a page.
   * @return void
   */
  virtual void activate();
  /**
   * Deactivate a page.
   * @return void
   */
  virtual void deactivate();
  /**
   * Draw the background.
   * @return void
   */
  virtual void draw_background();
  /**
   * Callback on resize.
   * @return void
   */
  virtual void on_resize();
  /**
   * Translate the page.
   * @return void
   */
  virtual void translate();
  /**
   * Update the home after stats changed.
   * @return void
   */
  void on_get_stats();
private:
  /**
   * Build the ui.
   * @return void
   */
  virtual void build_ui();
  /**
   * Build the background.
   * @return void
   */
  virtual void build_background();

  sfg::Image::Ptr img_league;
  sfg::Window::Ptr window_home;

  sf::RectangleShape bg;
};

#endif // __HOME_HPP__
/**@}*/