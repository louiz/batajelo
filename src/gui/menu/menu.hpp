#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

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
  void update(const sf::Time);
  void handle_event(const sf::Event&);

  void go_to_next_page();
  void go_to_previous_page();
  /**
   * Reset the light state of all the pages' widgets.
   * The light state is all the graphical effects (like fade out, or stuf)
   * that happens. But it does not reset the content of the page, for
   * example if a list of items was loaded into a page's widgets, the list
   * is kept untouched.
   */
  void reset_pages_state_light();

  /**
   * Reset the heavy state of all the pages' widgets
   * The heavy state is all the content of a page. It completely resets
   * the pages and make it just like when it was created.
   */
  void reset_pages_state_heavy();


private:
  sf::RenderWindow* win;
  Menu(const Menu&);
  Menu& operator=(const Menu&);

  std::vector<MenuPage*> pages;
  MenuPage* current_page;
};

#endif // __MENU_HPP__
