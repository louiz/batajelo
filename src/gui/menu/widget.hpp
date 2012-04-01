
#include <SFML/Graphics.hpp>

#ifndef __WIDGET_HPP__
# define __WIDGET_HPP__

class Widget
{
public:
  Widget(sf::RenderWindow*, unsigned int x, unsigned int y, unsigned width, unsigned height);
  ~Widget() {}
  virtual void draw() const = 0;
  virtual void on_mouse_button_event(sf::Event) = 0;
  bool contains(unsigned int, unsigned int);

protected:
  /**
   * The upper position of the widget
   */
  unsigned int x;
  /**
   * The left position of the widget
   */
  unsigned int y;
  /**
   * The width of the widget
   */
  unsigned int width;
  /**
   * The height of the widget
   */
  unsigned int height;

  sf::RenderWindow* win;

private:
  Widget(const Widget&);
  Widget& operator=(const Widget&);
  bool selected;
};


#endif // __WIDGET_HPP__
