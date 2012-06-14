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
  virtual ~DialogPopup() {}

  /**
   * Hide the page.
   * @return void
   */
  virtual void hide();
  /**
   * Show the popup.
   * @param box The box containing the pop-up elements.
   * @return void
   */
  void show(sfg::Box::Ptr);
private:
  /**
   * Build the ui.
   * @return void
   */
  virtual void build_ui();
};

#endif // __ERROR_POPUP_HPP__
