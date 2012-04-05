#include <gui/menu/text_input.hpp>
#include <logging/logging.hpp>

TextInput::TextInput(sf::RenderWindow* win,
		     unsigned int x, unsigned int y, unsigned int width, unsigned int height):
  Widget(win, x, y, width, height),
  cursor_pos(0),
  cursor_state(0)
{
  this->text.setCharacterSize(30);
  this->text.setColor(sf::Color::Green);
  this->update_text_position();
}

void TextInput::update_text_position()
{
  this->text.setString(this->string);
  this->center_text(this->text, 10);
}

void TextInput::draw() const
{
  this->draw_shape();
  this->draw_text();
}

void TextInput::update(const sf::Time dt)
{
  this->cursor_state += dt.asSeconds() * CURSOR_BLINK_SPEED;
  if (this->cursor_state > CURSOR_VISIBLE_TIME)
    this->cursor_state -= (CURSOR_VISIBLE_TIME + CURSOR_INVISBLE_TIME);
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
  this->win->draw(this->text);
}

void TextInput::on_text_entered(const sf::Event& event)
{
  if (event.text.unicode >= 32)
    {
      this->string += event.text.unicode;
    }
  else
    {
      log_debug(event.text.unicode);
      switch (event.text.unicode)
	{
	case 8:
	  this->delete_char();
	default:
	  break ;
	}
    }
  this->update_text_position();
}

void TextInput::delete_char()
{
  log_debug("delete_char");
  if (this->string.getSize())
    this->string.erase(this->string.getSize()-1);
  this->cursor_pos -= 1;
}
