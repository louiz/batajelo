#include <SFML/Graphics.hpp>
#include <vector>

#ifndef __MENU_PAGE_HPP__
# define __MENU_PAGE_HPP__

#include <gui/menu/widget.hpp>

class Menu;

class MenuPage
{
public:
  MenuPage(Menu*, sf::RenderWindow*);
  ~MenuPage() {}
  void draw() const;
  void update(const sf::Time);
  void add_widget(Widget*);
  void on_mouse_button_event(const sf::Event);
  void on_mouse_moved(const sf::Event);

  void reset_state_heavy();
  void reset_state_light();

protected:
  std::vector<Widget*> widgets;
  sf::RenderWindow* win;
  Menu* menu;

private:
  MenuPage(const MenuPage&);
  MenuPage& operator=(const MenuPage&);

};


#endif // __MENU_PAGE_HPP__
