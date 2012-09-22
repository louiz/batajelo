#ifndef __ACTION_PANEL_BUTTON_HPP__
# define __ACTION_PANEL_BUTTON_HPP__

#include <boost/function.hpp>
#include <SFML/Graphics.hpp>
#include <gui/cursor.hpp>

/**
 * A function called when we left click. We pass the world coordinate to it
 * (which should be ignored in some cases, for example if we target an
 * entity) and a type_id.
 */
typedef boost::function<bool(const unsigned int, const unsigned int, const std::size_t)> t_left_click_callback;

/**
 * a structure containing various elements to pass to the left click callback (for example if this is cast_spell, the structure contains the type_id of the spell).
 */
struct t_left_click
{
  /**
   * The callback called when we left click.
   */
  t_left_click_callback callback;
  /**
   * The type_id of the spell to cast or the unit to produce, etc
   */
  std::size_t id;
};

typedef boost::function<void(const t_left_click, const cursor::type)> t_action_panel_button_callback;

class ActionPanelButton
{
public:
  /**
   * TODO: read that from the mod file.
   */
  ActionPanelButton(const std::string&, const t_action_panel_button_callback, const std::size_t position, const t_left_click, const cursor::type);
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
  const t_left_click left_click;
  cursor::type cursor_type;
  sf::Texture texture;
  sf::Sprite sprite;
};

#endif // __ACTION_PANEL_PAGE_HPP__
