/** @addtogroup Ui
 *  @{
 */

/**
 * Settings
 * @class Settings
 */

#include <SFGUI/SFGUI.hpp>

#ifndef _SETTINGS_HPP__
# define __SETTINGS_HPP__

#include <gui/page.hpp>

class Settings: public Page
{
public:
  Settings(Ui*, sfg::Desktop*, sf::RenderWindow*);
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
private:
  /**
   * Build the ui.
   * @return void
   */
  virtual void build_ui();
  /**
   * Build the resolution ui.
   * @return void
   */
  void build_resolution();
  /**
   * Build the background.
   * @return void
   */
  virtual void build_background();
  /**
   * Update the settings.
   * @return void
   */
  void update();

  sfg::Window::Ptr window_settings;
  sfg::Image::Ptr img_settings;
  sfg::Image::Ptr img_back;
  sfg::Label::Ptr label_res;
  sfg::ComboBox::Ptr res;
  sfg::ComboBox::Ptr display;
};

#endif // __SETTINGS_HPP__
/**@}*/