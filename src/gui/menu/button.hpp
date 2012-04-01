#ifndef __BUTTON_HPP__
# define __BUTTON_HPP__

#include <gui/menu/widget.hpp>
#include <boost/function.hpp>

typedef boost::function<void(void)> t_button_callback;

class Button: public Widget
{
public:
Button(sf::RenderWindow*, unsigned int, unsigned int, unsigned int, unsigned int, const std::string&, t_button_callback);
  ~Button() {}
  virtual void draw() const;
  void draw_shape() const;
  void draw_text() const;
  virtual void on_mouse_button_event(sf::Event);
  void set_text(const std::string&);
  void update_position();

private:
  Button(const Button&);
  Button& operator=(const Button&);
  sf::Text text;
  t_button_callback callback;
};

#endif // __BUTTON_HPP__

