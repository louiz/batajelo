#include <world/entity_factory.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/vision.hpp>
#include <world/team.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/abilities.hpp>

#include <world/abilities/blink.hpp>

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

  auto abilities = std::make_unique<Abilities>(2);
  abilities->add(0, std::make_unique<Blink>());

  res->add_component(std::move(abilities));
  return res;
}
