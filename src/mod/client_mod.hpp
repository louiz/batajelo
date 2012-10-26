#ifndef __CLIENT_MOD_HPP__
# define __CLIENT_MOD_HPP__

#include <mod/mod.hpp>
#include <gui/sprites/building_sprite.hpp>

class Screen;

/**
 * Just a structure containing some numbers or string, describing an action,
 * it is filled with a YAML node.
 */
struct ModActionInfos
{
  std::string type;
  unsigned int page;
  unsigned int position;
  std::string image_filename;
  unsigned int value;
};

class ClientMod: public Mod
{
public:
  std::vector<ActionPanelTable*> get_action_tables(Screen*);
  std::vector<const BuildingSprite*> get_building_sprites();
  ClientMod(const std::string& filename);
private:
  ClientMod(const ClientMod&);
  ClientMod& operator=(const ClientMod&);

  void fill_default_unit_actions(Screen*, ActionPanelTable*);
  void fill_default_building_actions(Screen*, ActionPanelTable*);
  /**
   * Add empty pages until the table contains the given number of pages.
   * It may not add any pages if there are already enough of them.
   */
  void add_empty_pages(ActionPanelTable*, const std::size_t);
  void add_action_to_table(ActionPanelTable*, const ModActionInfos&, Screen*);

  std::map<const std::string, BuildingSprite*> building_sprites;
};

void operator >>(const YAML::Node& node, ModActionInfos& v);

#endif // __CLIENT_MOD_HPP__
