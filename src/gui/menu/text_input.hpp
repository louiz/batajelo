#ifndef __TEXT_INPUT_HPP__
# define __TEXT_INPUT_HPP__

#include <gui/menu/widget.hpp>

class TextInput: public Widget
{
public:
  TextInput(sf::RenderWindow*,
	    unsigned int, unsigned int, unsigned int, unsigned int);
  ~TextInput() {}
  virtual void draw() const;
  virtual void update(const sf::Time dt);
  virtual void reset_light();
  virtual void reset_heavy();
  virtual void on_text_entered(const sf::Event&);

protected:
  sf::String string;
  sf::Text text;

private:
  TextInput(const TextInput&);
  TextInput& operator=(const TextInput&);

  void draw_shape() const;
  void draw_text() const;
};


#endif // __TEXT_INPUT_HPP__
