#ifndef __SECOND_PAGE_HPP__
# define __SECOND_PAGE_HPP__

#include <gui/menu/menu_page.hpp>

class SecondPage: public MenuPage
{
public:
  SecondPage(Menu*, sf::RenderWindow*);
  ~SecondPage() {}

private:
  SecondPage(const SecondPage&);
  SecondPage& operator=(const SecondPage&);
};


#endif // __SECOND_PAGE_HPP__
