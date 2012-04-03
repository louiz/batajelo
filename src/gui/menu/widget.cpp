#include <gui/menu/widget.hpp>
#include <logging/logging.hpp>

Widget::Widget(sf::RenderWindow* win, unsigned int x, unsigned int y, unsigned width, unsigned height):
  x(x),
  y(y),
  width(width),
  height(height),
  win(win),
  selected(false)
{
  log_debug("Creating widget :" << x << ":" << y << " (" << width << ", " << height << ")");
}

bool Widget::contains(unsigned int x, unsigned int y)
{
  if ((x >= this->x) && (y >= this->y) &&
      (x <= this->x + this->width) && (y <= this->y + this->height))
    return true;
  return false;
}

void Widget::set_selected(const bool selected)
{
  if (selected != this->selected)
    {
      log_debug("Widget is now selected: " << selected);
      this->selected = selected;
    }
}
