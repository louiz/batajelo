#ifndef ENTITY_FACTORY_HPP_INCLUDED
#define ENTITY_FACTORY_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <map>

#include <world/unit.hpp>

static constexpr std::size_t MaxUnitModels = 32;

class EntityFactory
{
public:
  EntityFactory();
  ~EntityFactory();
  std::unique_ptr<Unit> make_unit(const EntityType type);

private:
  std::vector<Unit> unit_models;

  EntityFactory(const EntityFactory&) = delete;
  EntityFactory(EntityFactory&&) = delete;
  EntityFactory& operator=(const EntityFactory&) = delete;
  EntityFactory& operator=(EntityFactory&&) = delete;
};

#endif /* ENTITY_FACTORY_HPP_INCLUDED */
