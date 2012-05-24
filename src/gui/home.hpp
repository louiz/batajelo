/** @addtogroup Ui
 *  @{
 */

/**
 * Home
 * @class Home
 */

#include <SFGUI/SFGUI.hpp>
//#include <md5.h>

#ifndef __HOME_HPP__
# define __HOME_HPP__

#include <gui/page.hpp>

class Home: public Page
{
public:
  Home(Ui*, sfg::Desktop*, sf::RenderWindow*);
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
   * Build the login error ui.
   * @param error The error message.
   * @return void
   */
  void on_authenticate_error(const char*);
  /**
   * Build the login success ui.
   * @return void
   */
  void on_authenticate_success();
  void loading_animate();
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

  //MD5 md5;
  sfg::Label::Ptr label_error;
  sfg::Entry::Ptr entry_login;
  sfg::Entry::Ptr entry_pass;
  sfg::Window::Ptr window_login;
  sfg::Image::Ptr img_settings;
  sfg::Image::Ptr img_loading;
  sfg::Button::Ptr button_connect;
};

#endif // __HOME_HPP__
/**@}*/