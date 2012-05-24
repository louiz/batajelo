/** @addtogroup Ui
 *  @{
 */

/**
 * Page
 * @class Page
 */

#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#ifndef __PAGE_HPP__
# define __PAGE_HPP__

class Ui;

class Page
{
public:
  Page(Ui*, sfg::Desktop*, sf::RenderWindow*);
  virtual void build_ui() = 0;
  virtual void draw_background() = 0;
  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void activate() = 0;
  virtual void deactivate() = 0;


  sfg::Desktop* desktop;
  sf::RenderWindow* window;
  sf::Image img;
  sf::Sprite sprite_bg;
  Ui* ui;
};

#endif // __PAGE_HPP__
/**@}*/