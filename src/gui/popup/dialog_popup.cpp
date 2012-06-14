#include <gui/popup/dialog_popup.hpp>
#include <gui/ui.hpp>

DialogPopup::DialogPopup(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Popup(ui, desktop, window) 
{
}

void DialogPopup::hide()
{
  this->window_popup->Show(false);
}

void DialogPopup::show(sfg::Box::Ptr box)
{
  this->window_popup->Add(box);
  this->desktop->BringToFront(this->window_popup);
  this->window_popup->Show(true);
}

void DialogPopup::build_ui()
{
}