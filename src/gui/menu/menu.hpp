#include <vector>
#include <SFML/Graphics.hpp>

#ifndef __MENU_HPP__
# define __MENU_HPP__

#include <gui/menu/menu_page.hpp>

class Menu
{
public:
  Menu(sf::RenderWindow*);
  ~Menu() {}
  void add_page(MenuPage*);
  void set_current_page(MenuPage*);
  void draw() const;
  void handle_event(sf::Event);

  void go_to_next_page();
  void go_to_previous_page();

private:
  sf::RenderWindow* win;
  Menu(const Menu&);
  Menu& operator=(const Menu&);

  std::vector<MenuPage*> pages;
  MenuPage* current_page;
};

#endif // __MENU_HPP__
