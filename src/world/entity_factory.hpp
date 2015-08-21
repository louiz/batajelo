#ifndef ENTITY_FACTORY_HPP_INCLUDED
#define ENTITY_FACTORY_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <map>

#include <world/entity.hpp>

class World;

static constexpr std::size_t MaxEntityModels = 32;

class EntityFactory
{
public:
  EntityFactory();
  ~EntityFactory();
  std::unique_ptr<Entity> make_entity(World* world, const EntityType type);

private:
  std::vector<Entity> entity_models;

  EntityFactory(const EntityFactory&) = delete;
  EntityFactory(EntityFactory&&) = delete;
  EntityFactory& operator=(const EntityFactory&) = delete;
  EntityFactory& operator=(EntityFactory&&) = delete;
};

#endif /* ENTITY_FACTORY_HPP_INCLUDED */
