#include <gui/menu/text_input.hpp>
#include <logging/logging.hpp>

TextInput::TextInput(sf::RenderWindow* win,
		     unsigned int x, unsigned int y, unsigned int width, unsigned int height):
  Widget(win, x, y, width, height)
{
  this->text.setCharacterSize(30);
  this->text.setColor(sf::Color::Green);
  this->text.setPosition(this->x, this->y);
}

void TextInput::draw() const
{
  this->draw_shape();
  this->draw_text();
}

void TextInput::update(const sf::Time dt)
{
}

void TextInput::reset_light()
{
}

void TextInput::reset_heavy()
{
  this->string.clear();
}

void TextInput::draw_shape() const
{
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(this->width, this->height));
  if (this->selected)
    {
      rectangle.setOutlineColor(sf::Color(10, 10, 10));
      rectangle.setOutlineThickness(5);
    }
  else
    {
      rectangle.setOutlineColor(sf::Color(100, 100, 100));
      rectangle.setOutlineThickness(2);
    }
  rectangle.setPosition(this->x, this->y);
  this->win->draw(rectangle);
}

void TextInput::draw_text() const
{
  sf::Text t = this->text;
  t.setString(this->string);
  this->win->draw(t);
}

void TextInput::on_text_entered(const sf::Event& event)
{
  if (event.text.unicode >= 32)
    this->string += event.text.unicode;
}
