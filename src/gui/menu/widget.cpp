#include <gui/menu/widget.hpp>
#include <logging/logging.hpp>

Widget::Widget(sf::RenderWindow* win, unsigned int x, unsigned int y, unsigned width, unsigned height):
  win(win),
  x(x),
  y(y),
  width(width),
  height(height),
  selected(false)
{
  log_debug("Creating widget :" << x << ":" << y << " (" << width << ", " << height << ")");
}

bool Widget::contains(unsigned int x, unsigned int y)
{
  log_debug(x << ":" << y);
  if ((x >= this->x) && (y >= this->y) &&
      (x <= this->x + this->width) && (y <= this->y + this->height))
    return true;
  return false;
}
