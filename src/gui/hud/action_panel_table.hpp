/** @addtogroup Gui
 *  @{
 */


/**
 * An ActionPanelTable is associated with an entity, it contains all its possible
 * actions, dispatched in one or more ActionPage.  Each button of each
 * ActionPage is associated with an action. An action can be just “switch to
 * page x”, for example the button “build unit” switches to the page listing
 * all buildings that can be built by that entity.
 */

#ifndef __ACTION_PANEL_TABLE_HPP__
# define __ACTION_PANEL_TABLE_HPP__

#include <vector>
#include <gui/hud/action_panel_page.hpp>

class Screen;

class ActionPanelTable
{
public:
  ActionPanelTable(Screen*);
  ~ActionPanelTable();
  void draw(sf::RenderWindow*);
  bool handle_event(const sf::Event&);
  void action_change_current_page(const t_left_click_callback, const cursor::type, const std::size_t number);
  void change_current_page(const std::size_t number);

private:
  ActionPanelTable(const ActionPanelTable&);
  ActionPanelTable& operator=(const ActionPanelTable&);
  std::vector<ActionPanelPage*> pages;
  std::size_t current_page;
};

#endif // __ACTION_PANEL_TABLE_HPP__
