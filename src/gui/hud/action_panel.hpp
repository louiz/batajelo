/** @addtogroup Gui
 *  @{
 */

/**
 * An action panel contains an ordered vector of ActionTable. Each
 * actiontable is associated with an entity. For example if the selected
 * entity has the entity_id 2, the actiontable that should be displayed (and
 * used when clicking on the panel) should be action_tables[2].  In turn,
 * each ActionTable contains a list of one or more ActionPage, where actions
 * are associated with buttons.
 */

#ifndef __ACTION_PANEL_HPP__
# define __ACTION_PANEL_HPP__

#define ACTION_SQUARE_SIZE 68
#define ACTION_SQUARES_PER_LINE 2
#define ACTION_SPACE_BETWEEN_SQUARES 8
#define ACTION_SPACE_BETWEEN_LINES 2
#define ACTION_NUMBER_OF_LINES 3
#define ACTION_PANEL_X 1545
#define ACTION_PANEL_Y 845

// #define ACTION_TABS_X 622
// #define ACTION_TABS_Y 180
// #define ACTION_TAB_WIDTH 25
// #define ACTION_TAB_HEIGHT 15
// #define SPACE_BETWEEN_TABS 5


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <gui/hud/action_panel_table.hpp>
#include <world/selection.hpp>
#include <vector>

class Screen;

class ActionPanel
{
public:
  ActionPanel(sf::RenderWindow*, Screen*, const Selection*);
  ~ActionPanel();
  void draw();
  bool handle_event(const sf::Event&);
private:
  ActionPanel(const ActionPanel&);
  ActionPanel& operator=(const ActionPanel&);
  sf::RenderWindow* win;
  /**
   * A pointer to the Selection object contained in the ClientWorld object.
   */
  const Selection* const selection;
  std::vector<ActionPanelTable*> tables;
};

#endif // __ACTION_PANEL_HPP__

