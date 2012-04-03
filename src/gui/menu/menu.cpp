#include <gui/menu/menu.hpp>
#include <gui/menu/first_page.hpp>
#include <gui/menu/second_page.hpp>
#include <logging/logging.hpp>

Menu::Menu(sf::RenderWindow* win):
  win(win)
{
  log_debug("Creating menu");
  MenuPage* main_page = new FirstPage(this, win);
  this->add_page(main_page);

  MenuPage* second_page = new SecondPage(this, win);
  this->add_page(second_page);

  this->set_current_page(main_page);
}

void Menu::add_page(MenuPage* page)
{
  this->pages.push_back(page);
}

void Menu::set_current_page(MenuPage* page)
{
  this->current_page = page;
}

void Menu::draw() const
{
  this->win->clear(sf::Color(0, 0, 200));

  this->current_page->draw();
}

void Menu::update(const sf::Time dt)
{
  this->current_page->update(dt);
}

void Menu::handle_event(const sf::Event& event)
{
  if ((event.type == sf::Event::MouseButtonPressed) ||
      (event.type == sf::Event::MouseButtonPressed))
    this->current_page->on_mouse_button_event(event);
  else if (event.type == sf::Event::MouseMoved)
    this->current_page->on_mouse_moved(event);
  else if (event.type == sf::Event::TextEntered)
    this->current_page->on_text_entered(event);
}

void Menu::go_to_next_page()
{
  log_debug("go_to_next_page");
  this->reset_pages_state_light();
  this->set_current_page(this->pages[1]);
}

void Menu::go_to_previous_page()
{
  log_debug("go_to_previous_page");
  this->reset_pages_state_light();
  this->set_current_page(this->pages[0]);
}

void Menu::reset_pages_state_light()
{
  std::vector<MenuPage*>::const_iterator it;
  for (it = this->pages.begin(); it < this->pages.end(); ++it)
    {
      (*it)->reset_state_light();
    }
}

void Menu::reset_pages_state_heavy()
{
  std::vector<MenuPage*>::const_iterator it;
  for (it = this->pages.begin(); it < this->pages.end(); ++it)
    {
      (*it)->reset_state_heavy();
    }
}
