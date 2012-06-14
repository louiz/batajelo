/** @addtogroup Ui
 *  @{
 */

/**
 * LoginSteps
 * @class LoginSteps
 */

#include <SFGUI/SFGUI.hpp>

#ifndef __LOGIN_STEPS_HPP__
# define __LOGIN_STEPS_HPP__

#include <gui/page.hpp>

class LoginSteps: public Page
{
public:
  LoginSteps(Ui*, sfg::Desktop*, sf::RenderWindow*);
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
   * Initialize the labels.
   * @return void
   */
  void init();
  /**@{*/
  /**
   * @name Events
   */
  /**
   * Show the authentification success message.
   * @return void
   */
  void on_authenticate_success();
  /**
   * Show the connection success message.
   * @return void
   */
  void on_connection_success();
  /**@}*/
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

  sfg::Label::Ptr first_step;
  sfg::Label::Ptr second_step;
  sfg::Label::Ptr third_step;
  sfg::Window::Ptr window_steps;
  sfg::Button::Ptr cancel_connect;

  sf::RectangleShape bg;
};

#endif // __LOGIN_STEPS_HPP__
/**@}*/