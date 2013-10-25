#include <gui/hud/action_panel.hpp>

ActionPanel::ActionPanel(sf::RenderWindow* win, Screen* screen, const Selection* selection, ClientMod& mod):
  win(win),
  selection(selection)
{
  // for unit in MOD_FILE
  // create a nem ActionPanelTable()
  // add it to the vector

  // DO THAT
  this->tables = mod.get_action_tables(screen);

  // REMOVE THAT
  // ActionPanelTable* table = new ActionPanelTable(screen);
  // this->tables.push_back(table);
}

ActionPanel::~ActionPanel()
{
  std::vector<ActionPanelTable*>::iterator it;
}

void ActionPanel::draw()
{
  if (this->selection->is_empty())
    return;
  const Entity* entity_to_use = this->selection->get_entities().front();
  ActionPanelTable* table = this->tables[entity_to_use->type_id].get();
  table->draw(this->win);
}

bool ActionPanel::handle_event(const sf::Event& event)
{
  if (this->selection->is_empty())
    return false;
  const Entity* entity_to_use = this->selection->get_entities().front();
  ActionPanelTable* table = this->tables[entity_to_use->type_id].get();
  return table->handle_event(event);
}

void ActionPanel::reset_all_tables()
{
  std::vector<ActionPanelTable*>::iterator it;
  for (auto& table: this->tables)
    table->change_current_page(0);
}
