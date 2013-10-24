#include <mod/mod.hpp>
#include <gui/hud/action_panel_page.hpp>
#include <gui/hud/action_panel_button.hpp>
#include <gui/screen/screen.hpp>
#include <utils/make_unique.hpp>

#include <memory>

Mod::Mod(const std::string& filename):
    all_docs(YAML::LoadAllFromFile(filename))
{
}

Mod::~Mod()
{
}

std::vector<std::unique_ptr<Unit> > Mod::get_unit_models()
{
  std::vector<std::unique_ptr<Unit> > models;
  YAML::Node units_doc = this->all_docs[0];

  for (const auto& elem: units_doc)
    {
      auto unit = std::make_unique<Unit>();
      elem >> *unit;
      models.emplace_back(std::move(unit));
    }
  return models;
}

std::vector<std::unique_ptr<Building> > Mod::get_building_models()
{
  std::vector<std::unique_ptr<Building> > models;
  YAML::Node buildings_doc = this->all_docs[1];
  for (const auto& elem: buildings_doc)
    {
      auto building = std::make_unique<Building>();
      elem >> *building;
      models.emplace_back(std::move(building));
    }
  return models;
}

void operator >>(const YAML::Node& node, Fix16& v)
{
    v = node.as<float>();
}

void operator >>(const YAML::Node& node, Unit& v)
{
    v.name = node["name"].as<std::string>();
    v.speed = node["speed"].as<int>();
    v.width = node["width"].as<short>();
    v.spawn_duration = node["spawn_duration"].as<unsigned int>();
}

void operator >>(const YAML::Node& node, Building& v)
{
    v.name = node["name"].as<std::string>();
    v.build_time = node["build_time"].as<unsigned int>();
    v.cells = node["cells"].as<short>();
}
