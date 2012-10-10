/** @addtogroup Mod Defines a class that can provide a serie of various
 * objects or list of objects, defined in a mod file.  A mod file is a yaml
 * file describing various aspects and "rules" of the game (for example a
 * list of units and spells, their attributes, what unit can cast which
 * spell, etc).  This class only creates the object, the various class using
 * them are responsible for destroying them later (for example, the list of
 * Entity* that the World class requires has to be freed be the World
 * class).  One single mod object must be created each game and passed to
 * every class that needs it.
 *
 * A mod file is structured like this:
 * Two documents, in that order: the first contains a list of units
 * @class Mod
 */

#ifndef __MOD_HPP__
# define __MOD_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include <gui/hud/action_panel_table.hpp>
#include <world/unit.hpp>
#include <world/building.hpp>

#define NUMBER_OF_DOCUMENTS 1

class Mod
{
public:
  Mod(const std::string& filename);
  ~Mod();
  /**
   * Returns the list of all the entity models defined in the mod file.
   * These Entitys are then copied by World to create the real entities that
   * actually "live".
   */
  std::vector<const Unit*> get_unit_models();
  std::vector<const Building*> get_building_models();

private:
  Mod(const Mod&);
  Mod& operator=(const Mod&);

protected:
  std::ifstream fin;
  /**
   * Pointers to the various yaml documents in the mod file.
   */
  YAML::Node* units_doc;
  YAML::Node* buildings_doc;
};

void operator >>(const YAML::Node& node, mpreal& v);
void operator >>(const YAML::Node& node, Unit& v);
void operator >>(const YAML::Node& node, Building& v);

#endif // __MOD_HPP__
