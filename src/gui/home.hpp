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

#include <ui/page.hpp>

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
   * Build the ui.
	 * @return void
   */
	virtual void build_ui();
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
private:
	//MD5 md5;
	sfg::Label::Ptr label_login;
  sfg::Label::Ptr label_pass;
  sfg::Entry::Ptr entry_login;
  sfg::Entry::Ptr entry_pass;
	sfg::Window::Ptr window_login;
	sfg::Image::Ptr img_settings;
	/**
   * Connect to the game server
	 * @return void
   */
	void connect();	
};

#endif // __HOME_HPP__
/**@}*/