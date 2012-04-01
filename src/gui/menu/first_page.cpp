#include <gui/menu/first_page.hpp>
#include <gui/menu/button.hpp>
#include <gui/menu/menu.hpp>
#include <boost/bind.hpp>

FirstPage::FirstPage(Menu* menu, sf::RenderWindow* win):
  MenuPage(menu, win)
{
  Button* button = new Button(win, 10, 10, 300, 100, "Next", boost::bind(&Menu::go_to_next_page, this->menu));
  this->add_widget(button);
}
