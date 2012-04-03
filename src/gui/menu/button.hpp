#include <boost/function.hpp>

#ifndef __BUTTON_HPP__
# define __BUTTON_HPP__

#define FADE_SPEED 2

#include <gui/menu/widget.hpp>

typedef boost::function<void(void)> t_button_callback;

class Button: public Widget
{
public:
  Button(sf::RenderWindow*,
	 unsigned int, unsigned int, unsigned int, unsigned int,
	 const sf::String&, t_button_callback);
  ~Button() {}
  virtual void draw() const;
  virtual void update(const sf::Time);
  virtual void reset_light();
  virtual void reset_heavy();
  void draw_shape() const;
  void draw_text() const;
  virtual void on_mouse_button_event(const sf::Event&);
  void set_text(const std::string&);
  void update_position();

private:
  Button(const Button&);
  Button& operator=(const Button&);
  sf::Text text;
  t_button_callback callback;
  /**
   * A number between 0 and 1 changing slightly as the button
   * is hovered.
   */
  double color;
};

#endif // __BUTTON_HPP__

