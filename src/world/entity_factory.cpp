#include <world/entity_factory.hpp>
#include <world/health.hpp>
#include <world/mana.hpp>
#include <world/vision.hpp>

EntityFactory::EntityFactory()
{
  // this->unit_models.resize(MaxUnitModels);
  // First unit
  // this->unit_models[0].width = 10;
  // this->unit_models[0].speed = 2;
  // this->unit_models[0].spawn_duration = 40;
  // this->unit_models[0]. 
}

EntityFactory::~EntityFactory()
{
}

std::unique_ptr<Unit> EntityFactory::make_unit(const EntityType type)
{
  auto res = std::make_unique<Unit>();
  res->width = 10;
  res->speed = 2;
  res->spawn_duration = 40;
  res->add_component(std::make_unique<Health>(100));
  res->add_component(std::make_unique<Mana>(300));
  res->add_component(std::make_unique<Vision>(700, false));
  // return std::make_unique<Unit>(this->unit_models[type]);
  return res;
}
