#ifndef __TEXT_INPUT_HPP__
# define __TEXT_INPUT_HPP__

#include <gui/menu/widget.hpp>

#define CURSOR_BLINK_SPEED 1
#define CURSOR_INVISBLE_TIME 1
#define CURSOR_VISIBLE_TIME 1

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
  size_t cursor_pos;
  /**
   * Fluctuate between -CURSOR_INVISBLE_TIME and CURSOR_VISIBLE_TIME.
   * if < 0 the cursor is invisible.
   */
  char cursor_state;

private:
  TextInput(const TextInput&);
  TextInput& operator=(const TextInput&);
  void update_text_position();
  void delete_char();

  void draw_shape() const;
  void draw_text() const;
};


#endif // __TEXT_INPUT_HPP__
