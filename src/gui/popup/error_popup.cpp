#include <gui/popup/error_popup.hpp>
#include <gui/ui.hpp>

ErrorPopup::ErrorPopup(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): Popup(ui, desktop, window) 
{
  this->build_ui();
}

void ErrorPopup::hide()
{
  this->window_popup->Show(false);
}

void ErrorPopup::show(const std::string error)
{
  this->label_error->SetText(error);
  this->desktop->BringToFront(this->window_popup);
  this->window_popup->Show(true);
}

void ErrorPopup::build_ui()
{
  //Labels
  this->label_error = sfg::Label::Create();
  this->label_error->SetId("label_error");
  //Button
  sfg::Button::Ptr button_ok = sfg::Button::Create("ok");
  button_ok->SetId("button_connect");
  button_ok->GetSignal(sfg::Widget::OnLeftClick).Connect(&Ui::switch_to_login, this->ui);
  // Create box 
  this->box_popup->Pack(this->label_error);
  this->box_popup->SetSpacing(10);
  this->box_popup->Pack(button_ok, false);
  // Create window
  this->window_popup->Add(this->box_popup);
  // Add to desktop
  this->desktop->LoadThemeFromFile(this->ui->theme_path + "popup/error_popup.theme");
  this->desktop->Add(this->window_popup);
}