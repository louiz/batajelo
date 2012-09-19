#ifndef __ACTION_PANEL_PAGE_HPP__
# define __ACTION_PANEL_PAGE_HPP__

#include <vector>
#include <gui/hud/action_panel_button.hpp>

class ActionPanelPage
{
public:
  /**
   * TODO: read that from the mod file.
   */
  ActionPanelPage();
  ~ActionPanelPage();
  void add_button(ActionPanelButton* button, std::size_t position);
  void draw(sf::RenderWindow*);
  bool handle_event(const sf::Event&);
private:
  ActionPanelPage(const ActionPanelPage&);
  ActionPanelPage& operator=(const ActionPanelPage&);
  std::vector<ActionPanelButton*> buttons;
};

#endif // __ACTION_PANEL_PAGE_HPP__
