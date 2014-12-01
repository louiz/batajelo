#include <gui/screen/screen_element.hpp>

ScreenElement::ScreenElement(Screen* screen):
  screen(screen)
{
}

ScreenElement::~ScreenElement()
{
}

Screen* ScreenElement::get_screen()
{
  return this->screen;
}

void ScreenElement::set_priority(const int priority)
{
  this->priority = priority;
}

int ScreenElement::get_priority() const
{
  return this->priority;
}
