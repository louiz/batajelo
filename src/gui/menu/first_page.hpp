#ifndef __FIRST_PAGE_HPP__
# define __FIRST_PAGE_HPP__

#include <gui/menu/menu_page.hpp>

class FirstPage: public MenuPage
{
public:
  FirstPage(Menu*, sf::RenderWindow*);
  ~FirstPage() {}

private:
  FirstPage(const FirstPage&);
  FirstPage& operator=(const FirstPage&);
};


#endif // __FIRST_PAGE_HPP__
