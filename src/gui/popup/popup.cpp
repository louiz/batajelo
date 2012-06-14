#include <gui/popup/popup.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>

Popup::Popup(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window):
  desktop(desktop),
  window(window),
  ui(ui)
{
  // Box
  this->box_popup = sfg::Box::Create(sfg::Box::VERTICAL);
  this->box_popup->SetId("box_popup");
  // Window
  this->window_popup = sfg::Window::Create(sfg::Window::BACKGROUND);
  this->window_popup->SetId("window_popup");
  this->window_popup->SetPosition(sf::Vector2f(700.f*Config::get_int("width", 800)/1920.f, 400.f*Config::get_int("height", 600)/1080.f));
  this->window_popup->Show(false);
  this->build_background();
}

void Popup::build_background()
{
  this->bg.setSize(sf::Vector2f(Config::get_int("width", 800), Config::get_int("height", 600)));
  this->bg.setFillColor(sf::Color(128,128,128,100));
}

void Popup::draw_background()
{
  this->window->draw(this->bg);
}

void Popup::on_resize()
{
  this->window_popup->SetPosition(sf::Vector2f(700.f*Config::get_int("width", 800)/1920.f, 400.f*Config::get_int("height", 600)/1080.f));
  this->bg.setSize(sf::Vector2f(Config::get_int("width", 800), Config::get_int("height", 600)));
}
