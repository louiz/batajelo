/** @addtogroup Ui
 *  @{
 */

/**
 * ErrorPopup
 * @class ErrorPopup
 */

#include <SFGUI/SFGUI.hpp>

#ifndef __ERROR_POPUP_HPP__
# define __ERROR_POPUP_HPP__

#include <gui/popup/popup.hpp>

class ErrorPopup: public Popup
{
public:
  ErrorPopup(Ui*, sfg::Desktop*, sf::RenderWindow*);

  /**
   * Hide the page.
   * @return void
   */
  virtual void hide();
  /**
   * Show the page.
   * @param error The error message.
   * @return void
   */
  void show(const std::string error);
  /**
   * Build the ui.
   * @return void
   */
  virtual void build_ui();

  sfg::Label::Ptr label_error;

};

#endif // __ERROR_POPUP_HPP__