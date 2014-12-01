#include <world/entity_factory.hpp>

EntityFactory::EntityFactory()
{
  this->unit_models.resize(MaxUnitModels);
  // First unit
  this->unit_models[0].width = 10;
  this->unit_models[0].speed = 2;
  this->unit_models[0].spawn_duration = 40;
}

EntityFactory::~EntityFactory()
{
}

std::unique_ptr<Unit> EntityFactory::make_unit(const EntityType type)
{
  return std::make_unique<Unit>(this->unit_models[type]);
}
