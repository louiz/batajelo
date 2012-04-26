#include <world/world.hpp>

World::World()
{
  this->entities_iterator = this->entities.begin();
}

World::~World()
{
}

void World::load_test()
{
  Entity* unit = new Entity;
  this->insert_entity(unit);
}

Entity* World::get_next_entity()
{
  if (this->entities_iterator == this->entities.end())
    {
      this->entities_iterator = this->entities.begin();
      return 0;
    }
  Entity* entity = *this->entities_iterator;
  ++this->entities_iterator;
  return entity;
}

void World::reset_entity_iterator()
{
  this->entities_iterator = this->entities.begin();
}

void World::insert_entity(Entity* entity)
{
  this->entities.push_back(entity);
}
