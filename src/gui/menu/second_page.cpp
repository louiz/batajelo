#include <gui/menu/second_page.hpp>
#include <gui/menu/button.hpp>
#include <gui/menu/menu.hpp>
#include <boost/bind.hpp>

SecondPage::SecondPage(Menu* menu, sf::RenderWindow* win):
  MenuPage(menu, win)
{
  Button* button = new Button(win, 480, 10, 300, 100, L"Previous", boost::bind(&Menu::go_to_previous_page, this->menu));
  this->add_widget(button);
}
