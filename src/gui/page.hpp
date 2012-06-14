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

  virtual void draw_background() = 0;
  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void activate() = 0;
  virtual void deactivate() = 0;
  virtual void on_resize() = 0;
  virtual void translate() = 0;

  sfg::Desktop* desktop;
  sf::RenderWindow* window;
  sf::Texture bg;
  sf::Sprite sprite_bg;
  Ui* ui;
private:
  virtual void build_ui() = 0;
  virtual void build_background() = 0;
};

#endif // __PAGE_HPP__
/**@}*/