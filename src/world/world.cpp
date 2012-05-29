#include <world/world.hpp>

World::World():
  started(false)
{
  this->replay = new Replay;
  this->turn_handler = new TurnHandler(this->replay);
  this->init();
}

World::~World()
{
  delete this->turn_handler;
  if (this->replay != 0)
    delete this->replay;
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

void World::set_next_turn_callback(t_next_turn_callback callback)
{
  log_debug("set_next_turn_callback");
  this->turn_handler->set_next_turn_callback(callback);
}

void World::reset_entity_iterator()
{
  this->entities_iterator = this->entities.begin();
}

void World::insert_entity(Entity* entity)
{
  this->entities.push_back(entity);
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
  this->occupants.push_back(occupant);
  log_debug("Adding new occupant to the world:" << occupant->name << " " << occupant->number);
}

void World::init()
{
  log_debug("Init world");
  // TODO, load these units from the Mod file.
  Entity* unit = new Entity;
  this->entity_models.push_back(unit);

  log_debug("Done.");
}

Entity* World::create_entity(unsigned int type)
{
  const Entity* model = this->entity_models[type];
  Entity* new_entity = new Entity(*model);
  return new_entity;
}

void World::pause()
{
  this->turn_handler->pause();
}

void World::unpause()
{
  this->turn_handler->unpause();
}


void World::tick(bool force)
{
  if ((this->started == false) && (force == false))
    return ;
  this->turn_handler->tick(force);
  if (this->turn_handler->is_paused() == true)
    return ;
  Entity* entity;
  this->reset_entity_iterator();
  while ((entity = this->get_next_entity()))
    {
      entity->tick();
    }
}

void World::advance_replay_until_paused()
{
  do
    {
      this->tick(true);
    } while (this->turn_handler->is_paused() == false);
}

void World::start()
{
  log_debug("start");
  if (this->started == true)
    return;
  this->started = true;
}

void World::do_path(ActionEvent* event)
{
  PathEvent* path_event = static_cast<PathEvent*>(event);
  Path path(path_event->x, path_event->y);
  unsigned short entity_id = path_event->actors_ids[0];
  Entity* entity = this->get_entity_by_id(entity_id);
  entity->set_path(path);
}

void World::do_new_entity(ActionEvent* event)
{
  log_debug("DO NEW ENTITY");
  EntityEvent* entity_event = static_cast<EntityEvent*>(event);
  Entity* new_entity = entity_event->entity;
  log_debug("New_Entity" << new_entity->x << ":" << new_entity->y);
  this->insert_entity(new_entity);
}

void World::completely_validate_action(const unsigned int id)
{
  log_debug("Action " << id << " completely validated.");
  return this->turn_handler->completely_validate_action(id);
}

Entity* World::get_entity_by_id(unsigned short id)
{
  // Should use something like this->entities[id], to optimize.
  // Entities should be placed directly in a vector, for fast access.
  Entity* entity;
  this->reset_entity_iterator();
  while ((entity = this->get_next_entity()))
    {
      if (entity->get_id() == id)
	return entity;
    }
  return 0;
}

void World::validate_turn_completely(const unsigned int number)
{
  this->turn_handler->completely_validate_turn(number);
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

Replay* World::get_replay() const
{
  return this->replay;
}

TurnHandler* World::get_turn_handler() const
{
  return this->turn_handler;
}

unsigned int World::get_number_of_occupants() const
{
  return this->occupants.size();
}
