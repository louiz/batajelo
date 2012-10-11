#include <boost/bind.hpp>

#include <gui/screen/screen.hpp>
#include <mod/client_mod.hpp>
#include <world/client_world.hpp>

#include <gui/sprites/archive_sprite.hpp>

ClientMod::ClientMod(const std::string& filename):
  Mod(filename)
{
  this->building_sprites["building_archive"] = new ArchiveSprite;
}


std::vector<ActionPanelTable*> ClientMod::get_action_tables(Screen* screen)
{
  ModActionInfos action_infos;
  std::vector<ActionPanelTable*> tables;
  log_warning("heu, coucou: " << this->units_doc->size());
  for (unsigned int i=0; i < this->units_doc->size(); i++)
    {
      const YAML::Node& unit = (*this->units_doc)[i];
      ActionPanelTable* table = new ActionPanelTable;
      tables.push_back(table);
      log_warning("Adding a new table, for a unit");
      // A table always contains at least ONE page
      this->fill_default_unit_actions(screen, table);
      const YAML::Node& actions = unit["actions"];
      for (unsigned int i=0; i < actions.size(); i++)
        {
          const YAML::Node& action = actions[i];
          action >> action_infos;
          this->add_action_to_table(table, action_infos, screen);
        }
    }

  // YAML::Node buildings;
  // parser.GetNextDocument(units);

  return tables;
}
std::vector<const BuildingSprite*> ClientMod::get_building_sprites()
{
  std::vector<const BuildingSprite*> sprites;
  std::map<const std::string, BuildingSprite*>::iterator it;
  std::string sprite_name;
  for (unsigned int i=0; i < this->buildings_doc->size(); i++)
    {
      (*this->buildings_doc)[i]["sprite"] >> sprite_name;
      it = this->building_sprites.find(sprite_name);
      if (it == this->building_sprites.end())
        {
          log_error("Can not find the sprite for building: " << sprite_name << ". Will probably segfault later on because of that.");
          continue ;
        }
      sprites.push_back(it->second);
    }
  return sprites;
}

void ClientMod::add_empty_pages(ActionPanelTable* table, const std::size_t size)
{
  while (table->pages.size() < size)
    table->pages.push_back(new ActionPanelPage);
}

void ClientMod::fill_default_unit_actions(Screen* screen, ActionPanelTable* table)
{
  this->add_empty_pages(table, 1);
  ActionPanelPage* page = table->pages[0];

  const t_left_click null_left = {0, 0, 0};
  t_left_click left_click = null_left;

  // Move button
  left_click.callback = boost::bind(&ClientWorld::action_move, screen->get_world(), _1, _2, _3);
  left_click.cursor_callback = boost::bind(&Screen::draw_move_cursor, screen, _1, _2, _3);
  page->add_button(new ActionPanelButton("move.png",
                                         boost::bind(&Screen::set_left_click_callback, screen, _1), 0, left_click), 0);

  // Empty buttons, yet
  page->add_button(new ActionPanelButton("stop.png",
                                         0, 1, null_left), 1);
  page->add_button(new ActionPanelButton("keep_position.png",
                                         0, 2, null_left), 2);
  page->add_button(new ActionPanelButton("patrol.png",
                                         0, 3, null_left), 3);
  page->add_button(new ActionPanelButton("attack.png",
                                         0, 4, null_left), 4);
}

void ClientMod::add_action_to_table(ActionPanelTable* table, const ModActionInfos& infos, Screen* screen)
{
  t_left_click left_click = {0, 0, 0};
  ActionPanelButton* button;

  this->add_empty_pages(table, infos.page + 1); // To can add the button into its page.
  ActionPanelPage* page = table->pages[infos.page];
  if (infos.type == "page")
    {
      this->add_empty_pages(table, infos.value + 1); // To be able to switch to the page.
      // Indicate what page we switch to when clicking the button.
      left_click.id = infos.value;
      button = new ActionPanelButton(infos.image_filename, boost::bind(&ActionPanelTable::action_change_current_page, table, _1), infos.position, left_click);
    }
  else if (infos.type == "build")
    {
      left_click.id = infos.value;
      left_click.callback = boost::bind(&ClientWorld::action_build, screen->get_world(), _1, _2, _3);
      left_click.cursor_callback = boost::bind(&Screen::draw_build_cursor, screen, _1, _2, _3);
      button = new ActionPanelButton(infos.image_filename, boost::bind(&Screen::set_left_click_callback, screen, _1), infos.position, left_click);
    }
  else
    {
      log_error("Unknown type of action: " << infos.type);
      return ;
    }
  page->add_button(button, infos.position);
}

void operator >>(const YAML::Node& node, ModActionInfos& v)
{
  node["type"] >> v.type;
  node["page"] >> v.page;
  node["position"] >> v.position;
  node["value"] >> v.value;
  node["image"] >> v.image_filename;
}

