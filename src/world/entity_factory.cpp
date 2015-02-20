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
#include <world/abilities/phase.hpp>
#include <world/abilities/emp.hpp>

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
      entity->make_manipulable();
      entity->add_component(std::make_unique<Health>(100));
      entity->add_component(std::make_unique<ManaPool>(300));
      entity->add_component(std::make_unique<Vision>(700, false));
      entity->add_component(std::make_unique<Location>(20, true));
      entity->add_component(std::make_unique<Team>());
      entity->add_component(std::make_unique<Mobility>(2.2_fix));
      entity->add_component(std::make_unique<Acquisition>(200_fix));

      auto abilities = std::make_unique<Abilities>(4u, 20u, 20u);
      abilities->add(0, std::make_unique<Attack>(20u, 20u));
      abilities->add(1, std::make_unique<Blink>());
      abilities->add(2, std::make_unique<Phase>());
      abilities->add(3, std::make_unique<Emp>());

      entity->add_component(std::move(abilities));
    }
  else if (type == 1)
    { // Some test projectile
      entity->add_component(std::make_unique<Location>(2, false));
      entity->add_component(std::make_unique<Mobility>(9));
      entity->add_component(std::make_unique<Team>());
    }
  else if (type == 2)
    { // Emp
      entity->add_component(std::make_unique<Location>(0, false));
      entity->add_component(std::make_unique<Team>());
    }
  return entity;
}
