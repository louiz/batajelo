#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#ifndef __WIDGET_HPP__
# define __WIDGET_HPP__

class Widget
{
public:
  Widget(sf::RenderWindow*,
	 unsigned int, unsigned int, unsigned int, unsigned int);
  ~Widget() {}

  virtual void draw() const = 0;
  virtual void update(const sf::Time) = 0;
  /**
   * Reset only the temporary states of the widget (for
   * example fade out effects, or its position, etc)
   */
  virtual void reset_light() = 0;
  /**
   * Reset all states of the widgets. For example if a content
   * was added since its creation, we remove it.
   */
  virtual void reset_heavy() = 0;

  virtual void on_mouse_button_event(const sf::Event&) {};
  virtual void on_mouse_moved(const sf::Event&) {};
  virtual void on_text_entered(const sf::Event&) {};
  virtual void on_key_pressed(const sf::Event&) {};
  virtual void on_mouse_enter() {};
  virtual void on_mouse_leave() {};
  bool contains(unsigned int, unsigned int);

  void set_hovered(const bool);
  void set_selected(const bool);
  bool get_selected() const;

protected:
  /**
   * Takes a text object and set its position to be centered
   * in the widget.
   */
  void center_text(sf::Text&, int x = -1, int y = -1);

  /**
   * The upper position of the widget
   */
  unsigned int x;
  /**
   * The left position of the widget
   */
  unsigned int y;
  /**
   * The width of the widget
   */
  unsigned int width;
  /**
   * The height of the widget
   */
  unsigned int height;

  sf::RenderWindow* win;

  bool selected;
  bool hovered;
private:
  Widget(const Widget&);
  Widget& operator=(const Widget&);
};

#endif // __WIDGET_HPP__
