/** @addtogroup Gui
 *  @{
 */

/**
   TODO update this comment once I have decided on a design.
 * Contains all details about how the list of entities in the current
 * selection is displayed in the hud. It keeps the list internally and is
 * updated whenever the world::current_selection changes. User events should
 * be passed for it to to handle clicks on unit-squares, hovers, tab
 * changes, etc.
 *
 * @class SelectionPanel
 */

#ifndef __SELECTION_PANEL_HPP__
# define __SELECTION_PANEL_HPP__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <game/selection.hpp>
#include <game/time.hpp>
#include <gui/screen/screen_element.hpp>

#define SQUARE_SIZE 50
#define SQUARES_PER_LINE 10
#define SPACE_BETWEEN_SQUARES 8
#define SPACE_BETWEEN_LINES 8
#define LINES_PER_TAB 3
#define PANEL_X 650
#define PANEL_Y 190

#define TABS_X 622
#define TABS_Y 180
#define TAB_WIDTH 25
#define TAB_HEIGHT 15
#define SPACE_BETWEEN_TABS 5

class GameClient;

class SelectionPanel: public ScreenElement
{
public:
  SelectionPanel(GameClient* game, Screen* screen, const Selection* selection);
  ~SelectionPanel();
  void draw() override final;
  bool handle_event(const sf::Event&) override final;
  void update(const Duration& dt) override final;
  void draw_tab(const std::size_t);
  bool is_entity_hovered(const Entity*) const;
  const Entity* get_entity_under_mouse() const;

private:
  SelectionPanel(const SelectionPanel&);
  SelectionPanel& operator=(const SelectionPanel&);
  std::size_t current_tab;
  GameClient* game;
  /**
   * A pointer to the Selection object contained in the ClientWorld object.
   */
  const Selection* const selection;
};

#endif // __SELECTION_PANEL_HPP__
