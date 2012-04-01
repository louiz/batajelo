#include <vector>

#include <gui/menu/menu_page.hpp>
#include <logging/logging.hpp>
#include <gui/menu/menu.hpp>

MenuPage::MenuPage(Menu* menu, sf::RenderWindow* win):
  menu(menu),
  win(win)
{
  log_debug("Creating MenuPage");
}

void MenuPage::draw() const
{
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      (*it)->draw();
    }
}

void MenuPage::add_widget(Widget* widget)
{
  this->widgets.push_back(widget);
}

void MenuPage::on_mouse_button_event(sf::Event event)
{
  log_debug("MenuPage::on_mouse_button_event");
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      if ((*it)->contains(event.mouseButton.x, event.mouseButton.y) == true)
	(*it)->on_mouse_button_event(event);
    }
}
