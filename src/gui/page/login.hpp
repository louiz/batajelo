/** @addtogroup Ui
 *  @{
 */

/**
 * Login
 * @class Login
 */

#include <SFGUI/SFGUI.hpp>

#ifndef __LOGIN_HPP__
# define __LOGIN_HPP__

#include <gui/page/page.hpp>

class Login: public Page
{
public:
  Login(Ui*, sfg::Desktop*, sf::RenderWindow*);
  virtual ~Login() {}

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
   * Callback on connection success
   * @return void
   */
  void on_authenticate_success();
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
  void invalidate();
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
  /**
   * Connect to the game server
   * @return void
   */
  void connect();  
  /**
   * Save the login
   * @return void
   */
  void save_login();  
  /**
   * Handle key events for login field
   * @return void
   */
  void handle_keys_login();  
  /**
   * Handle key events for password field
   * @return void
   */
  void handle_keys_password();
  /**
   * Handle key events for connect button
   * @return void
   */
  void handle_keys_connect();

  sfg::Entry::Ptr entry_login;
  sfg::Entry::Ptr entry_pass;
  sfg::Label::Ptr label_login;
  sfg::Label::Ptr label_pass;
  sfg::Window::Ptr window_login;
  sfg::Window::Ptr window_menu;
  sfg::Image::Ptr img_loading;
  sfg::CheckButton::Ptr remember_login;
  sfg::Button::Ptr button_login;
  sfg::Button::Ptr button_close;
  sfg::Button::Ptr button_settings;
  sf::Image settings;
  sfg::Image::Ptr img_refresh;

  sf::RectangleShape bg;
};

#endif // __LOGIN_HPP__
/**@}*/