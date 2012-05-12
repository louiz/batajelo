#include <world/world.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <game/event.hpp>

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
      MoveEvent event;
      Entity* entity;
      while ((entity = this->get_next_entity()))
	{
	  if (entity->is_selected())
	    event.actors_ids.push_back(entity->get_id());
	}
      event.x = x;
      event.y = y;
      this->generate_command("MOVE", event.to_string());
    }
}

void World::generate_command(const char* name, const std::string& archive)
{
  Command* command = new Command;
  command->set_name(name);
  command->set_body(archive.data(), archive.length());
  this->commands_queue.push(command);
  log_debug(this->commands_queue.size());
}

Command* World::get_pending_command()
{
  if (this->commands_queue.empty())
    return 0;
  Command* command = this->commands_queue.front();
  this->commands_queue.pop();
  return command;
}

void World::try_move(Command* command)
{
  MoveEvent event;
  event.from_string(std::string(command->body, command->body_size));

  
}
