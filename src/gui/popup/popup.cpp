#include <gui/popup/popup.hpp>
#include <config/config.hpp>

Popup::Popup(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window): ui(ui), desktop(desktop), window(window) 
{
  this->box_popup = sfg::Box::Create(sfg::Box::VERTICAL);
  this->box_popup->SetId("box_popup");

  this->window_popup = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_popup->SetId("window_popup");
}

void Popup::draw_background()
{
  this->bg.setSize(sf::Vector2f(Config::get_int("width", 800), Config::get_int("height", 600)));
  this->bg.setFillColor(sf::Color(128,128,128,100));
  this->window->draw(this->bg);
}