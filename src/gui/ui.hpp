/** @addtogroup Ui
 *  @{
 */

/**
 * User Interface
 * @class Ui
 */

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>
#include <boost/locale.hpp>

#ifndef __UI_HPP__
# define __UI_HPP__

#include <gui/page.hpp>
#include <network/command.hpp>

class Popup;
class ErrorPopup;
class DialogPopup;
class Game;
class LoginSteps;
class Settings;
class Login;

class Ui
{
public:
	Ui(Game*);
	~Ui();
  /**
   * Initialize translation
   * @return void
   */
  void init_translation();
	/**@{*/
  /**
   * @name Window
   */
  /**
   * Close the window.
   * @return void
   */
  void close();
  /**
   * Handle all window events.
   * @return void
   */
  void handle_event();
  /**
   * Get event_handled value.
   * @return bool
   */
  bool get_event_handled();
  /**
   * Set event_handled value.
   * @return bool
   */
  void set_event_handled(bool);
  /**
   * Handle all window events.
   * @return void
   */
  void on_resize();
  /**
   * Clear the window.
   * @return void
   */
  void clear();
  /**
   * Refresh the window.
   * @return void
   */
  void refresh();
  /**
   * Check if the window is still open.
   * @return bool
   */
  bool run();
  /**
   * Draw the ui background
   * @return void
   */
  void draw_background();
  /**
   * Draw the popup background
   * @return void
   */
  void draw_background_popup();
  /**
   * Display the ui.
   * @return void
   */
  void display_ui();
  /**
   * Display the background.
   * @return void
   */
  void display_background();
  /**@}*/
  /**@{*/
  /**
   * @name Pages
   */
  /**
   * Load all the ui pages.
   * @return void
   */
  void load_all_pages();
  /**
   * Page switcher.
   * @param page A page pointer.
   * @return void
   */
  void switch_to_page(Page*);
  /**
   * Switch to login page.
   * @return void
   */
  void switch_to_login();
  /**
   * Switch to login steps page.
   * @return void
   */
  void switch_to_login_steps();
  /**
   * Switch to settings page.
   * @return void
   */
  void switch_to_settings();
  /**@}*/
  /**@{*/
  /**
   * @name Popups
   */
  /**
   * Load all the ui popups.
   * @return void
   */
  void load_all_popups();
  /**
   * Switcher to Error Popup
   * @param error The error message.
   * @return void
   */
  void switch_to_error_popup(const std::string);
  /**
   * Switcher to Dialog Popup.
   * @param error The dialog message.
   * @return void
   */
  void switch_to_dialog_popup(sfg::Box::Ptr);
  /**@}*/
  /**@{*/
  /**
   * @name Events
   */
  /**
   * Connect to server with login/password.
   * @return void
   */
  void on_login_form_validated(const std::string&, const std::string&, const std::string&, const short&);
  /**
   * Callback after connection to server failed.
   * @return void
   */
  void on_connection_success();
  /**
   * Callback after connection to server succeed.
   * @return void
   */
  void on_connection_failed();
  /**
   * Callback after trying to login
   * @return void
   */
  void on_authenticate(Command *);
  /**@}*/
  /**
   * Get the display mode based on config.
   * @return int
   */
  int get_display_mode();
  /**
   * Get the current key pressed
   * @return sf::Keyboard::Key
   */
  sf::Keyboard::Key get_key();
  /**
   * Set the language
   * @return void
   */
  void set_language(std::string);
  /**
   * Translate the ui
   * @return void
   */
  void translate();

  const std::string font_path;
  const std::string theme_path;
  const std::string img_path;
private:
  Page* current_page;
  Login* login;
  LoginSteps* login_steps;
  Settings* settings;
  Popup* current_popup;
  ErrorPopup* error_popup;
  DialogPopup* dialog_popup;
  sfg::Desktop* desktop;
  sf::RenderWindow* window;
  sf::Event event;
  sf::Clock clock;
  sfg::SFGUI sfgui;
  Game* game;
  bool event_handled;
  boost::locale::generator gen;
};

#endif // __UI_HPP__
/**@}*/
