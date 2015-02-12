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
  auto entity = std::make_unique<Entity>(type);
  if (type == 0)
    { // Default unit
      entity->add_component(std::make_unique<Health>(100));
      entity->add_component(std::make_unique<ManaPool>(300));
      entity->add_component(std::make_unique<Vision>(700, false));
      entity->add_component(std::make_unique<Location>(10));
      entity->add_component(std::make_unique<Team>());
      entity->add_component(std::make_unique<Mobility>(2));
      entity->add_component(std::make_unique<Acquisition>(200_fix));

      auto abilities = std::make_unique<Abilities>(2u, 20u, 20u);
      abilities->add(0, std::make_unique<Attack>(20u, 20u));
      abilities->add(1, std::make_unique<Blink>());

      entity->add_component(std::move(abilities));
    }
  else if (type == 1)
    { // Some test projectile
      entity->add_component(std::make_unique<Location>(2));
      entity->add_component(std::make_unique<Mobility>(7));
      entity->add_component(std::make_unique<Team>());
    }
  return entity;
}
