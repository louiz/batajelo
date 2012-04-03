#include <vector>

#include <gui/menu/menu_page.hpp>
#include <logging/logging.hpp>
#include <gui/menu/menu.hpp>

MenuPage::MenuPage(Menu* menu, sf::RenderWindow* win):
  win(win),
  menu(menu)
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

void MenuPage::update(const sf::Time dt)
{
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      (*it)->update(dt);
    }
}

void MenuPage::add_widget(Widget* widget)
{
  this->widgets.push_back(widget);
}

void MenuPage::on_mouse_button_event(const sf::Event event)
{
  log_debug("MenuPage::on_mouse_button_event");
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      if ((*it)->contains(event.mouseButton.x, event.mouseButton.y) == true)
	(*it)->on_mouse_button_event(event);
    }
}

void MenuPage::on_mouse_moved(const sf::Event event)
{
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      if ((*it)->contains(event.mouseMove.x, event.mouseMove.y) == true)
	(*it)->set_selected(true);
      else
	(*it)->set_selected(false);
    }

}

void MenuPage::reset_state_light()
{
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      (*it)->set_selected(false);
      (*it)->reset_light();
    }
}

void MenuPage::reset_state_heavy()
{
  std::vector<Widget*>::const_iterator it;
  for (it = this->widgets.begin(); it < this->widgets.end(); ++it)
    {
      (*it)->reset_heavy();
    }
}

