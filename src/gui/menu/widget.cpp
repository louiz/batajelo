#include <gui/menu/widget.hpp>
#include <logging/logging.hpp>

Widget::Widget(sf::RenderWindow* win, unsigned int x, unsigned int y, unsigned width, unsigned height):
  x(x),
  y(y),
  width(width),
  height(height),
  win(win),
  selected(false),
  hovered(false)
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

void Widget::set_hovered(const bool hovered)
{
  if (hovered != this->hovered)
    {
      log_debug("Widget is now hovered: " << hovered);
      this->hovered = hovered;
    }
}

void Widget::set_selected(const bool selected)
{
  if (selected != this->selected)
    {
      log_debug("Widget is now selected: " << selected);
      this->selected = selected;
    }
}

bool Widget::get_selected() const
{
  return this->selected;
}

void Widget::center_text(sf::Text& text, int x, int y)
{				// this sucks
  sf::FloatRect rect = text.getGlobalBounds();
  if (x == -1)
    x = this->x + this->width/2 - rect.width/2;
  else
    x += this->x;
  if (y == -1)
    y = this->y + this->height/2 - rect.height/2;
  else
    y += this->y;
  text.setPosition(x, y);
  log_debug(rect.height << " " << rect.width);
}
