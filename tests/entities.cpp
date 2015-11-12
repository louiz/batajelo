#include "catch.hpp"

#include <world/entity_factory.hpp>
#include <world/health.hpp>
#include <world/location.hpp>
#include <world/mobility.hpp>
#include <world/abilities.hpp>
#include <world/abilities/blink.hpp>
#include <world/abilities/emp.hpp>
#include <world/abilities/attack.hpp>

SCENARIO("Create entities and check their behaviour", "[entities]")
{
  GIVEN("a basic entity")
    {
      Entity entity(0);

      WHEN("we add basic components")
        {
          entity.add_component(std::make_unique<Health>(10));
          entity.add_component(std::make_unique<Location>(10, true));
          THEN("We can get these components")
            {
              auto location = entity.get<Location>();
              CHECK(location);
              auto health = entity.get<Health>();
              CHECK(health);
            }
          AND_THEN("We can see that it does not have the other components")
            {
              auto mobility = entity.get<Mobility>();
              CHECK(!mobility);
            }
        }
      WHEN("we add an empty abilities component")
        {
          entity.add_component(std::make_unique<Abilities>(2));
          THEN("it has an ability component")
            {
              auto abilities = entity.get<Abilities>();
              CHECK(abilities);
            }
        }
    }
}

TEST_CASE("Create and fill Abilities", "[entities]")
{
  GIVEN("An empty entity")
    {
      Entity entity(0);
      WHEN("we add components and abilities in one line")
        {
          entity.add_component<Location>(10, true).
              add_and_get_component<Abilities>().
              add<Attack>(300ms, 500ms, 450_fix).
              add<Emp>();
          THEN("the entity has all the given attributes")
            {
              auto abilities = entity.get<Abilities>();
              CHECK(abilities);
              auto attack = abilities->find<Attack>(AbilityType::Attack);
              CHECK(attack);
              CHECK(attack->get_range() == 450_fix);
              auto blink = abilities->find<Blink>(AbilityType::Blink);
              CHECK(!blink);
            }
        }
      AND_WHEN("we create an other entity")
        {
          Entity entity2(0);
          THEN("the new ID is different")
            {
              CHECK(entity2.get_id() != entity.get_id());
            }
        }
    }
}

TEST_CASE("Entity factory", "[entities]")
{
  EntityFactory factory;
  Entity entity(0);

  auto entity_ptr = factory.make_entity(0);
  CHECK(entity_ptr->get_type() == entity.get_type());
}
