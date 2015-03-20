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
#include <world/abilities/dash.hpp>
#include <world/abilities/concentrate.hpp>

#include <utils/time.hpp>

using namespace std::chrono_literals;

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
      entity->add_component(std::make_unique<Health>(200));
      entity->add_component(std::make_unique<ManaPool>(300));
      entity->add_component(std::make_unique<Vision>(700, false));
      entity->add_component(std::make_unique<Location>(20, true));
      entity->add_component(std::make_unique<Team>());
      entity->add_component(std::make_unique<Mobility>(3.8_fix));

      auto abilities = std::make_unique<Abilities>(6u, 0u, 0u);
      abilities->add(0, std::make_unique<Attack>(300ms, 500ms, 400_fix));
      abilities->add(1, std::make_unique<Blink>());
      abilities->add(2, std::make_unique<Phase>());
      abilities->add(3, std::make_unique<Emp>());
      abilities->add(4, std::make_unique<Dash>(500, 50));
      abilities->add(5, std::make_unique<Concentrate>());

      entity->add_component(std::move(abilities));
    }
  else if (type == 1)
    { // Some test projectile
      entity->add_component(std::make_unique<Location>(2, false));
      entity->add_component(std::make_unique<Mobility>(11));
      entity->add_component(std::make_unique<Team>());
    }
  else if (type == 2)
    { // Emp
      entity->add_component(std::make_unique<Location>(0, false));
      entity->add_component(std::make_unique<Team>());
    }
  return entity;
}
