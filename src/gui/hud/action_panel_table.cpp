#include <boost/bind.hpp>

#include <gui/hud/action_panel_table.hpp>
#include <gui/screen/screen.hpp>
#include <world/client_world.hpp>
#include <gui/cursor.hpp>

ActionPanelTable::ActionPanelTable(Screen* screen):
  current_page(0)
{
  // for page in action of the given entity of the MOD_FILE
  // create page
  // add new ActionButton()s to page
  // add page to the vector
  ActionPanelPage* page = new ActionPanelPage();
  this->pages.push_back(page);
  ClientWorld* world = screen->get_world();
  
  t_left_click_callback left_click_callback = boost::bind(&ClientWorld::action_move, world, _1, _2);
  const std::size_t pos = 0;
  ActionPanelButton* move_button = new ActionPanelButton("./data/images/actions/move.png",
                                                         boost::bind(&Screen::set_left_click_callback, screen, _1, _2), pos, left_click_callback, cursor::Move);
  page->add_button(move_button, pos);
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
