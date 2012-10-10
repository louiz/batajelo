#include <boost/bind.hpp>

#include <gui/hud/action_panel_table.hpp>
#include <gui/cursor.hpp>

ActionPanelTable::ActionPanelTable():
  current_page(0)
{
}

ActionPanelTable::~ActionPanelTable()
{
}

void ActionPanelTable::draw(sf::RenderWindow* win)
{
  this->pages[this->current_page]->draw(win);
}

bool ActionPanelTable::handle_event(const sf::Event& event)
{
  return this->pages[this->current_page]->handle_event(event);
}

void ActionPanelTable::action_change_current_page(const t_left_click left_click)
{
  const std::size_t number = left_click.id;
  assert(number <= this->pages.size() - 1);
  this->change_current_page(number);
}

void ActionPanelTable::change_current_page(const std::size_t number)
{
  this->current_page = number;
}
