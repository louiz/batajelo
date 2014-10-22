#include <functional>
#include <logging/logging.hpp>
#include <gui/screen/screen.hpp>
#include <mod/client_mod.hpp>
#include <world/client_world/client_world.hpp>

#include <gui/sprites/archive_sprite.hpp>
#include <gui/common.hpp>

const std::string building_texture_path("data/images/buildings/");

ClientMod::ClientMod(const std::string& filename):
  Mod(filename)
{
}

std::vector<std::unique_ptr<ActionPanelTable>> ClientMod::get_action_tables(Screen* screen)
{
  std::vector<std::unique_ptr<ActionPanelTable>> tables;

  // YAML::Node units_doc = this->all_docs[0];
  // for (const auto& unit: units_doc)
  //   {
  //     std::unique_ptr<ActionPanelTable> table = std::make_unique<ActionPanelTable>();
  //     log_warning("Adding a new table, for a unit");
  //     this->fill_default_unit_actions(screen, table.get());
  //     for (const auto& action: unit["actions"])
  //       {
  //         ModActionInfos action_infos(action);
  //         this->add_action_to_table(table.get(), action_infos, screen);
  //       }
  //     tables.emplace_back(std::move(table));
  //   }

  // YAML::Node buildings_doc = this->all_docs[1];
  // for (const auto& building: buildings_doc)
  //   {
  //     std::unique_ptr<ActionPanelTable> table = std::make_unique<ActionPanelTable>();
  //     this->fill_default_building_actions(screen, table.get());
  //     for (const auto& action: building["actions"])
  //       {
  //         ModActionInfos action_infos(action);
  //         this->add_action_to_table(table.get(), action_infos, screen);
  //       }
  //     tables.emplace_back(std::move(table));
  //   }
  return tables;
}

std::vector<std::unique_ptr<sf::Texture>> ClientMod::get_building_textures()
{
  std::vector<std::unique_ptr<sf::Texture>> building_textures;
  YAML::Node buildings_doc = this->all_docs[1];
  for (const auto& building: buildings_doc)
    {
      std::string sprite_name = building["sprite"].as<std::string>();
      std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
      if (!texture->loadFromFile(building_texture_path + sprite_name + ".png"))
        throw GraphInitError();
      building_textures.emplace_back(std::move(texture));
    }
  return building_textures;
}

void ClientMod::add_empty_pages(ActionPanelTable* table, const std::size_t size)
{
  while (table->pages.size() < size)
    table->pages.push_back(new ActionPanelPage);
}

void ClientMod::fill_default_unit_actions(Screen* screen, ActionPanelTable* table)
{
  // static const t_left_click null_left = {nullptr, nullptr, 0};

  // this->add_empty_pages(table, 1);
  // ActionPanelPage* page = table->pages[0];

  // t_left_click left_click = null_left;

  // // Move button
  // left_click.callback = std::bind(&GameClient::action_move, screen->get_world(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  // left_click.cursor_callback = std::bind(&Screen::draw_move_cursor, screen, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  // page->add_button(new ActionPanelButton("move.png",
  //                                        std::bind(&Screen::set_left_click_callback, screen, std::placeholders::_1), 0, left_click), 0);

  // // Empty buttons, yet
  // page->add_button(new ActionPanelButton("stop.png",
  //                                        0, 1, null_left), 1);
  // page->add_button(new ActionPanelButton("keep_position.png",
  //                                        0, 2, null_left), 2);
  // page->add_button(new ActionPanelButton("patrol.png",
  //                                        0, 3, null_left), 3);
  // page->add_button(new ActionPanelButton("attack.png",
  //                                        0, 4, null_left), 4);
}

void ClientMod::fill_default_building_actions(Screen*, ActionPanelTable* table)
{
  this->add_empty_pages(table, 1);
}

void ClientMod::add_action_to_table(ActionPanelTable* table, const ModActionInfos& infos, Screen* screen)
{
  // t_left_click left_click = {0, 0, 0};
  // ActionPanelButton* button;

  // this->add_empty_pages(table, infos.page + 1); // To can add the button into its page.
  // ActionPanelPage* page = table->pages[infos.page];
  // if (infos.type == "page")
  //   {
  //     this->add_empty_pages(table, infos.value + 1); // To be able to switch to the page.
  //     // Indicate what page we switch to when clicking the button.
  //     left_click.id = infos.value;
  //     button = new ActionPanelButton(infos.image_filename, std::bind(&ActionPanelTable::action_change_current_page, table, std::placeholders::_1), infos.position, left_click);
  //   }
  // else if (infos.type == "build")
  //   {
  //     left_click.id = infos.value;
  //     left_click.callback = std::bind(&ClientWorld::action_build, screen->get_world(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  //     left_click.cursor_callback = std::bind(&Screen::draw_build_cursor, screen, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
  //     button = new ActionPanelButton(infos.image_filename, std::bind(&Screen::set_left_click_callback, screen, std::placeholders::_1), infos.position, left_click);
  //   }
  // else if (infos.type == "spawn")
  //   {
  //     left_click.id = infos.value;
  //     left_click.callback = 0;
  //     left_click.cursor_callback = 0;
  //     button = new ActionPanelButton(infos.image_filename, std::bind(&ClientWorld::action_spawn, screen->get_world(), std::placeholders::_1), infos.position, left_click);
  //   }
  // else
  //   {
  //     log_error("Unknown type of action: " << infos.type);
  //     return ;
  //   }
  // page->add_button(button, infos.position);
}
