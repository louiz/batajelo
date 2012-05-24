/** @addtogroup Ui
 *  @{
 */

/**
 * User Interface
 * @class Ui
 */

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#ifndef __UI_HPP__
# define __UI_HPP__

#include <gui/page.hpp>
#include <network/command.hpp>

class Popup;
class ErrorPopup;
class Game;
class Settings;
class Home;

class Ui
{
public:
	Ui(Game*);
	~Ui();
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
  void handleEvent();
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
   * Switch to home page.
   * @return void
   */
  void switch_to_home();
  /**
   * Switch to settings page.
   * @return void
   */
  void switch_to_settings();
   /**@}*/
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
  void switch_to_error_popup(const std::string error);
   /**@}*/

  /**
   * Connect to server with login/password
   * @return void
   */
  void on_login_form_validated(const std::string&, const std::string&, const std::string&, const short&);
  /**
   * Callback after trying to login
   * @return void
   */
  void on_authenticate(Command *);
  /**
   * Get the display mode based on config
   * @return int
   */
  int get_display_mode();
  /**
   * Get the current key pressed
   * @return sf::Keyboard::Key
   */
  sf::Keyboard::Key get_key();

  const std::string font_path;
  const std::string theme_path;
  const std::string img_path;
  private:
  Page* current_page;
  Home* home;
  Settings* settings;
  Popup* current_popup;
  ErrorPopup* error_popup;
  sfg::Desktop* desktop;
  sf::RenderWindow* window;
  sf::Event event;
  sf::Clock clock;
  sfg::SFGUI sfgui;
  Game* game;
};

#endif // __UI_HPP__
/**@}*/
