#include <logging/logging.hpp>
#include <gui/menu/button.hpp>

Button::Button(sf::RenderWindow* win, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const sf::String& text, t_button_callback callback):
  Widget(win, x, y, width, height),
  callback(callback),
  color(0)
{
  this->text.setCharacterSize(42);
  this->text.setColor(sf::Color::Red);
  this->set_text(text);
}

void Button::draw() const
{
  this->draw_shape();
  this->draw_text();
}

void Button::update(const sf::Time dt)
{
  if (this->hovered && this->color < 1)
    {
      this->color += dt.asSeconds() * FADE_SPEED;
      if (this->color > 1)
	this->color = 1;
    }
  else if (!this->hovered && this->color > 0)
    {
      this->color -= dt.asSeconds() * FADE_SPEED;
      if (this->color < 0)
	this->color = 0;
    }
}

void Button::reset_light()
{
  this->color = 0;
}

void Button::reset_heavy()
{

}

void Button::draw_shape() const
{
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(this->width, this->height));
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setFillColor(sf::Color(100*this->color, 0, 80*this->color));
  rectangle.setOutlineThickness(3);
  rectangle.setPosition(this->x, this->y);
  this->win->draw(rectangle);
}

void Button::draw_text() const
{
  this->win->draw(this->text);
}

void Button::on_mouse_button_event(const sf::Event& event)
{
  if (event.mouseButton.button == 0)
    this->callback();
}

void Button::set_text(const std::string& text)
{
  this->text.setString(text);
  this->update_position();
}

void Button::update_position()
{
  // TODO, find a way to center the text.
  this->center_text(this->text);
  // this->text.setPosition((this->x + this->width/2) - (this->text.getCharacterSize() / 2),
  // 			 (this->y + this->height / 2) - (this->text.getCharacterSize() / 2));
}
