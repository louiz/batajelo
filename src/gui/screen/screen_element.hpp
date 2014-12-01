#ifndef SCREEN_ELEMENT_HPP_INCLUDED
#define SCREEN_ELEMENT_HPP_INCLUDED

#include <SFML/Window.hpp>
#include <game/time.hpp>

class Screen;

class ScreenElement
{
public:
  explicit ScreenElement(Screen* screen);
  virtual ~ScreenElement();
  /**
   * The element draws itself in the screen
   */
  virtual void draw() = 0;
  /**
   * Update the element by the given amount
   */
  virtual void update(const Duration& dt) = 0;
  /**
   * Handle the given event.  We may decide te “eat” it by returning true in
   *  that case other ScreenElements with lower priority will not receive
   *  this event.
   */
  virtual bool handle_event(const sf::Event& event) = 0;

  Screen* get_screen();

  void set_priority(const int priority);
  int get_priority() const;

protected:
  /**
   * Non-owning screen pointer, mostly to call draw primitives.
   */
  Screen* screen;

private:
  int priority;

  ScreenElement(const ScreenElement&) = delete;
  ScreenElement(ScreenElement&&) = delete;
  ScreenElement& operator=(const ScreenElement&) = delete;
  ScreenElement& operator=(ScreenElement&&) = delete;
};

#endif /* SCREEN_ELEMENT_HPP_INCLUDED */
