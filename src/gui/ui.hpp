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
   * Update desktop of sgui.
	 * @return void
   */
	void update();
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
   * Pages switcher.
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
   * Connect to server with login/password
   * @return void
   */
	void on_login_form_validated(const std::string&, const std::string&, const std::string&, const short&);
	void on_authenticate(Command *);

	std::string font_path;
	std::string theme_path;
  std::string img_path;
private:
	Page* current_page;
	Home* home;
	Settings* settings;
	sfg::Desktop* desktop;
  sf::RenderWindow* window;
	sf::Event event;
  sf::Clock clock;
	sfg::SFGUI sfgui;
	Game* game;
};

#endif // __UI_HPP__
/**@}*/
