#ifndef __ACTION_PANEL_BUTTON_HPP__
# define __ACTION_PANEL_BUTTON_HPP__

#include <boost/function.hpp>
#include <SFML/Graphics.hpp>
#include <gui/cursor.hpp>

/**
 * A function called when we left click. We pass the world coordinate to it
 * (which should be ignored if we are clicking on a hud element instead of
 * somewhere on the world).
 */
typedef boost::function<bool(const unsigned int, const unsigned int)> t_left_click_callback;


typedef boost::function<void(const t_left_click_callback, const cursor::type, const std::size_t)> t_action_panel_button_callback;

class ActionPanelButton
{
public:
  /**
   * TODO: read that from the mod file.
   */
  ActionPanelButton(const std::string&, const t_action_panel_button_callback, const std::size_t position, const t_left_click_callback, const cursor::type, const std::size_t);
  ~ActionPanelButton();
  void draw(sf::RenderWindow*);
  void on_clicked();

  int x;
  int y;

private:
  ActionPanelButton(const ActionPanelButton&);
  ActionPanelButton& operator=(const ActionPanelButton&);
  /**
   * The action to execute whenever the button is pressed.
   */
  const t_action_panel_button_callback callback;
  const std::size_t position;
  const t_left_click_callback left_click_callback;
  cursor::type cursor_type;
  const std::size_t value;
  sf::Texture texture;
  sf::Sprite sprite;
};

#endif // __ACTION_PANEL_PAGE_HPP__
