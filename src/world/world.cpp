#include <world/world.hpp>

World::World()
{
  this->entities_iterator = this->entities.begin();
  this->load_test();
}

World::~World()
{
}

void World::load_test()
{
  Entity* unit = new Entity;
  this->insert_entity(unit);
  unit = new Entity();
  unit->x = 40;
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

void World::handle_event(actions::Type type, unsigned int x, unsigned y)
{
  if (type == actions::Select)
    {
      Entity* entity;
      while ((entity = this->get_next_entity()))
	{
	  if (entity->contains(x, y))
	    entity->selected = true;
	  else
	    entity->selected = false;
	}
    }
  else if (type == actions::Move)
    {
      Action* action = new Action(actions::Move);
      Entity* entity;
      while ((entity = this->get_next_entity()))
	{
	  if (entity->is_selected())
	    action->actors_ids.push_back(entity->get_id());
	}
      action->x = x;
      action->y = y;
      this->generate_action(action);
    }
}

void World::generate_action(const Action* action)
{
  this->actions_queue.push(action);
  log_debug(this->actions_queue.size());
}

const Action* World::get_pending_action()
{
  if (this->actions_queue.size() == 0)
    return 0;
  const Action* action = this->actions_queue.front();
  this->actions_queue.pop();
  return action;
}
