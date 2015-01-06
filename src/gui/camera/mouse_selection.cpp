#include <gui/camera/mouse_selection.hpp>

MouseSelection::MouseSelection():
  ongoing(false)
{
}

void MouseSelection::start(const sf::Vector2i& pos)
{
  this->start_pos = pos;
  this->ongoing = true;
}

void MouseSelection::end()
{
  // assert(this->ongoing == true);
  this->ongoing = false;
}

bool MouseSelection::contains(const sf::Vector2i& mouse_pos,
                              const sf::Vector2i& pos,
                              const uint width) const
{
  sf::Rect<uint> mouse_selection;
  if (this->ongoing)
    {
      if ((this->start_pos.x < mouse_pos.x))
        mouse_selection.left = this->start_pos.x;
      else
        mouse_selection.left = mouse_pos.x;
      if ((this->start_pos.y < mouse_pos.y))
        mouse_selection.top = this->start_pos.y;
      else
        mouse_selection.top = mouse_pos.y;
      mouse_selection.top += width;
      mouse_selection.left += width/2;
      mouse_selection.width = ::abs(this->start_pos.x - mouse_pos.x);
      mouse_selection.height = ::abs(this->start_pos.y - mouse_pos.y);
    }
  else
    {
      mouse_selection.left = mouse_pos.x + width/2;
      mouse_selection.top = mouse_pos.y + width;
      mouse_selection.width = 1;
      mouse_selection.height = 1;
    }
  sf::Rect<uint> rectangle(pos.x, pos.y, width, width);
  return rectangle.intersects(mouse_selection);
  // if ((this->start_pos.x < mouse_pos.x))
  //   {
  //     if (((x - width/2) < this->start_pos.x) || ((x + width/2) > mouse_pos.x))
  //       return false;
  //   }
  // else if (((x + width/2) > this->start_pos.x) || ((x - width/2) < mouse_pos.x))
  //   return false;
  // if ((this->start_pos.y < mouse_pos.y))
  //   {
  //     if (((y - width/2) < this->start_pos.y) || ((y + width/2) > mouse_pos.y))
  //       return false;
  //   }
  // else if (((y + width/2) > this->start_pos.y) || ((y - width/2) < mouse_pos.y))
  //   return false;
  // return true;
}
