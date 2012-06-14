/** @addtogroup Ui
 *  @{
 */

/**
 * Settings
 * @class Settings
 */

#include <SFGUI/SFGUI.hpp>

#ifndef __SETTINGS_HPP__
# define __SETTINGS_HPP__

#include <gui/page.hpp>

class Settings: public Page
{
public:
  Settings(Ui*, sfg::Desktop*, sf::RenderWindow*);
  virtual ~Settings() {}
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
   * Callback event on resize.
   * @return void
   */
  virtual void on_resize();
  /**
   * Translate the settings page.
   * @return void
   */
  virtual void translate();
private:
  /**
   * Build the ui.
   * @return void
   */
  virtual void build_ui();
  /**
   * Get the current language.
   * @return void
   */
  int get_language();
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
   * Build the language vector
   * @return void
   */
  void build_languages();
  /**
   * Update the settings.
   * @return void
   */
  void update();
  /**
   * Allow the change of resolution
   * @return void
   */
  void change_resolution();

  sfg::Notebook::Ptr notebook;
  sfg::Window::Ptr window_settings;
  sfg::Image::Ptr img_settings;
  sfg::Image::Ptr img_back;
  sfg::Label::Ptr label_res;
  sfg::Label::Ptr label_display;
  sfg::Label::Ptr label_language;
  sfg::Label::Ptr label_table_video;
  sfg::Label::Ptr label_table_account;
  sfg::Button::Ptr button_settings_video;
  sfg::Button::Ptr button_settings_account;
  sfg::ComboBox::Ptr res;
  sfg::ComboBox::Ptr display;
  sfg::ComboBox::Ptr language;
  sfg::Table::Ptr table_video;
  sfg::Table::Ptr table_account;
  std::map<std::string, std::string> language_codes;

  sf::RectangleShape bg;
};

#endif // __SETTINGS_HPP__
/**@}*/
