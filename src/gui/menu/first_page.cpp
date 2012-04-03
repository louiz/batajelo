#include <gui/menu/first_page.hpp>
#include <gui/menu/button.hpp>
#include <gui/menu/text_input.hpp>
#include <gui/menu/menu.hpp>
#include <boost/bind.hpp>

FirstPage::FirstPage(Menu* menu, sf::RenderWindow* win):
  MenuPage(menu, win)
{
  Button* button = new Button(win, 10, 10, 300, 100, sf::String(L"Next"), boost::bind(&Menu::go_to_next_page, this->menu));
  TextInput* input = new TextInput(win, 400, 350, 350, 50);
  TextInput* input2 = new TextInput(win, 20, 350, 350, 50);
  this->add_widget(button);
  this->add_widget(input);
  this->add_widget(input2);
}
