#ifndef CLIENT_MOD_HPP
# define CLIENT_MOD_HPP

#include <memory>

#include <mod/mod.hpp>
#include <gui/sprites/building_sprite.hpp>

class Screen;

/**
 * A structure containing some numbers or string, describing an action,
 * it is filled with a YAML node.
 */
struct ModActionInfos
{
  ModActionInfos(const YAML::Node& node):
    type(node["type"].as<std::string>()),
    image_filename(node["image"].as<std::string>()),
    page(node["page"].as<unsigned int>()),
    position(node["position"].as<unsigned int>()),
    value(node["value"].as<unsigned int>())
  {}
  std::string type;
  std::string image_filename;
  unsigned int page;
  unsigned int position;
  unsigned int value;
};

class ClientMod: public Mod
{
public:
  ClientMod(const std::string& filename);
  std::vector<std::unique_ptr<ActionPanelTable>> get_action_tables(Screen*);
  std::vector<std::unique_ptr<sf::Texture>> get_building_textures();
private:
  void fill_default_unit_actions(Screen*, ActionPanelTable*);
  void fill_default_building_actions(Screen*, ActionPanelTable*);
  /**
   * Add empty pages until the table contains the given number of pages.
   * It may not add any pages if there are already enough of them.
   */
  void add_empty_pages(ActionPanelTable*, const std::size_t);
  void add_action_to_table(ActionPanelTable*, const ModActionInfos&, Screen*);

  ClientMod(const ClientMod&) = delete;
  ClientMod& operator=(const ClientMod&) = delete;
  ClientMod(ClientMod&&) = delete;
  ClientMod& operator=(ClientMod&&) = delete;
};

#endif // CLIENT_MOD_HPP
