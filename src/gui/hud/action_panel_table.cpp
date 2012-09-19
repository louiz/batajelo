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
  ActionPanelButton* move_button = new ActionPanelButton("./data/images/actions/move.png",
                                                         boost::bind(&Screen::set_left_click_callback, screen, _1, _2, _3), 0, left_click_callback, cursor::Move, 0);
  page->add_button(move_button, 0);

  page->add_button(new ActionPanelButton("./data/images/actions/build2.png",
                                         boost::bind(&ActionPanelTable::action_change_current_page, this, _1, _2, _3), 11, 0, cursor::Normal, 1),
                   11);
  page->add_button(new ActionPanelButton("./data/images/actions/stop.png",
                                         0, 1, 0, cursor::Normal, 0),
                   1);

  page->add_button(new ActionPanelButton("./data/images/actions/keep_position.png",
                                         0, 2, 0, cursor::Normal, 0),
                   2);

  page->add_button(new ActionPanelButton("./data/images/actions/patrol.png",
                                         0, 3, 0, cursor::Normal, 0),
                   3);

  page->add_button(new ActionPanelButton("./data/images/actions/attack.png",
                                         0, 4, 0, cursor::Normal, 0),
                   4);

  page = new ActionPanelPage();
  this->pages.push_back(page);
  move_button = new ActionPanelButton("./data/images/actions/move.png",
                                                         boost::bind(&Screen::set_left_click_callback, screen, _1, _2, _3), 12, left_click_callback, cursor::Move, 0);
  page->add_button(move_button, 12);


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

void ActionPanelTable::action_change_current_page(const t_left_click_callback, const cursor::type, const std::size_t number)
{
  assert(number <= this->pages.size() - 1);
  this->change_current_page(number);
}

void ActionPanelTable::change_current_page(const std::size_t number)
{
  this->current_page = number;
}
