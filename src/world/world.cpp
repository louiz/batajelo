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

void World::new_occupant_callback(Command* command)
{
  log_debug("new_occupant_callback");
  Occupant* occupant = new Occupant;

  std::string data(command->body, command->body_size);
  occupant->from_string(data);
  log_debug("occupant: " << occupant->name << " " << occupant->number);
  this->add_new_occupant(occupant);
}

void World::occupant_left_callback(Command* command)
{
  Occupant occupant;
  occupant.from_string(std::string(command->body, command->body_size));
  log_debug("Occupant to remove: " << occupant.number);
  this->remove_occupant(&occupant);
}

void World::remove_occupant(Occupant* occupant)
{
  std::vector<Occupant*>::iterator it;
  Occupant* occupant_to_remove;

  for (it = this->occupants.begin(); it < this->occupants.end(); ++it)
    {
      if ((*it)->number == occupant->number)
	{
	  occupant_to_remove = (*it);
	  this->occupants.erase(it);
	  delete occupant_to_remove;
	  return ;
	}
    }
  assert(false);
}

void World::add_new_occupant(Occupant* occupant)
{
  log_debug("Adding no occupant to the world:" << occupant->name << " " << occupant->number);
  this->occupants.push_back(occupant);
}
