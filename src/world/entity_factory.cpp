#include <world/entity_factory.hpp>
#include <world/health.hpp>
#include <world/manapool.hpp>
#include <world/vision.hpp>
#include <world/team.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/abilities.hpp>
#include <world/acquisition.hpp>

#include <world/abilities/blink.hpp>
#include <world/abilities/attack.hpp>

EntityFactory::EntityFactory()
{
}

EntityFactory::~EntityFactory()
{
}

std::unique_ptr<Entity> EntityFactory::make_entity(const EntityType type)
{
  auto res = std::make_unique<Entity>(type);
  res->add_component(std::make_unique<Health>(100));
  res->add_component(std::make_unique<ManaPool>(300));
  res->add_component(std::make_unique<Vision>(700, false));
  res->add_component(std::make_unique<Location>(10));
  res->add_component(std::make_unique<Team>());
  res->add_component(std::make_unique<Mobility>(2));
  res->add_component(std::make_unique<Acquisition>(200_fix));

  auto abilities = std::make_unique<Abilities>(2u, 20u, 20u);
  abilities->add(0, std::make_unique<Attack>(20u, 20u));
  abilities->add(1, std::make_unique<Blink>());

  res->add_component(std::move(abilities));
  return res;
}
