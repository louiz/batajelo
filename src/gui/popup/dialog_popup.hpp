/** @addtogroup Ui
 *  @{
 */

/**
 * DialogPopup
 * @class DialogPopup
 */

#include <SFGUI/SFGUI.hpp>

#ifndef __DIALOG_POPUP_HPP__
# define __DIALOG_POPUP_HPP__

#include <gui/popup/popup.hpp>

class DialogPopup: public Popup
{
public:
  DialogPopup(Ui*, sfg::Desktop*, sf::RenderWindow*);

  /**
   * Hide the page.
   * @return void
   */
  virtual void hide();
  /**
   * Build the ui.
   * @return void
   */
  virtual void build_ui();
};

#endif // __ERROR_POPUP_HPP__