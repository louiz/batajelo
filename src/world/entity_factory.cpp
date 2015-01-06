#include <world/entity_factory.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/vision.hpp>
#include <world/team.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>

EntityFactory::EntityFactory()
{
}

EntityFactory::~EntityFactory()
{
}

std::unique_ptr<Entity> EntityFactory::make_entity(const EntityType)
{
  auto res = std::make_unique<Entity>();
  res->add_component(std::make_unique<Health>(100));
  res->add_component(std::make_unique<ManaPool>(300));
  res->add_component(std::make_unique<Vision>(700, false));
  res->add_component(std::make_unique<Location>(10));
  res->add_component(std::make_unique<Team>());
  res->add_component(std::make_unique<Mobility>(2));

  return res;
}
