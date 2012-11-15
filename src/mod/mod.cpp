#include <mod/mod.hpp>
#include <gui/hud/action_panel_page.hpp>
#include <gui/hud/action_panel_button.hpp>
#include <gui/screen/screen.hpp>

Mod::Mod(const std::string& filename):
  fin(filename.c_str()),
  units_doc(new YAML::Node),
  buildings_doc(new YAML::Node)
{
  YAML::Parser parser(this->fin);
  parser.GetNextDocument(*this->units_doc);
  parser.GetNextDocument(*this->buildings_doc);
}

Mod::~Mod()
{
  delete this->units_doc;
  delete this->buildings_doc;
}

std::vector<const Unit*> Mod::get_unit_models()
{
  std::vector<const Unit*> models;
  Unit* unit;
  for (unsigned int i=0; i < this->units_doc->size(); i++)
    {
      unit = new Unit;
      (*this->units_doc)[i] >> *unit;
      models.push_back(unit);
    }
  return models;
}

std::vector<const Building*> Mod::get_building_models()
{
  std::vector<const Building*> buildings;
  Building* building;
  for (unsigned int i=0; i < this->buildings_doc->size(); i++)
    {
      building = new Building;
      (*this->buildings_doc)[i] >> *building;
      buildings.push_back(building);
    }
  return buildings;
}

void operator >>(const YAML::Node& node, mpreal& v)
{
  float f;
  node >> f;
  v = f;
}

void operator >>(const YAML::Node& node, Unit& v)
{
  node["name"] >> v.name;
  node["speed"] >> v.speed;
  node["width"] >> v.width;
  node["spawn_duration"] >> v.spawn_duration;
}

void operator >>(const YAML::Node& node, Building& v)
{
  node["name"] >> v.name;
  node["build_time"] >> v.build_time;
  node["cells"] >> v.cells;
}
