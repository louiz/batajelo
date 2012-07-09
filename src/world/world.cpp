#include <world/world.hpp>
#include <world/serializable_entity.hpp>

World::World():
  started(false),
  map(0)
{
  this->replay = new Replay;
  this->turn_handler = new TurnHandler(this->replay);
  this->init();
}

World::World(Map* map):
  started(false),
  map(map)
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
  this->reset_entity_iterator();
  Entity* entity;
  while ((entity = this->get_next_entity()) != 0)
    delete entity;
  std::vector<Entity*>::iterator it;
  for (it = this->entity_models.begin(); it < this->entity_models.end(); ++it)
    delete (*it);
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

Entity* World::get_next_entity(const int y)
{
  if (this->entities_iterator == this->entities.end())
    {
      this->entities_iterator = this->entities.begin();
      return 0;
    }
  Entity* entity = *this->entities_iterator;
  if (entity->y < y)
    {
      ++this->entities_iterator;
      return entity;
    }
  return 0;
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

Entity* World::create_entity(unsigned int type, const SerializableEntity& e)
{
  log_debug("Creating entity of type " << type);
  const Entity* model = this->entity_models[type];
  Entity* new_entity = new Entity(*model, e);
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
  if (this->map == 0)
    {
      this->map = new Map();
      this->map->load_from_file("test3.tmx");
    }
  this->started = true;
}

void World::do_path(ActionEvent* event)
{
  PathEvent* path_event = static_cast<PathEvent*>(event);
  Path path(path_event->x, path_event->y);
  std::vector<unsigned short>::const_iterator actors_it;
  for (actors_it = path_event->actors_ids.begin(); actors_it < path_event->actors_ids.end(); ++actors_it)
    {
      Entity* entity = this->get_entity_by_id((*actors_it));
      assert(entity != 0);
      entity->set_path(path);
    }
}

void World::do_new_entity(ActionEvent* event)
{
  log_debug("DO NEW ENTITY");
  EntityEvent* entity_event = static_cast<EntityEvent*>(event);
  // This SerializableEntity just contains the initial position of the
  // entity, and it's type_id because we don't need to pass all these
  // informations, because we already have all the possible entity types in
  // our list (entity_models).
  SerializableEntity* entity = entity_event->entity;
  // We use the type id to create an entity using the corresponding model,
  // and we pass the initial position of the entity as well.
  Entity* new_entity = this->create_entity(entity->type_id, *entity);
  delete entity;
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

static bool compare_entities(const Entity* a, const Entity* b)
{
  return (a->y < b->y);
}

void World::sort_entities()
{
  this->entities.sort(compare_entities);
}

void World::get_cell_at_position(const mpreal& x, const mpreal& y,
                                 int& xa, int& ya) const
{
  const int16_t cell_size = static_cast<const int16_t>(CELL_SIZE);
  assert(cell_size % 2 == 0);
  assert(cell_size > 1);
  xa = (x / cell_size).toLong();
  ya = (y / cell_size).toLong();
}

mpreal World::get_position_height(const mpreal& x, const mpreal& y) const
{
  int cellx;
  int celly;
  const int cell_size = static_cast<const int>(CELL_SIZE);
  this->get_cell_at_position(x, y, cellx, celly);
  ushort heights = this->map->get_cell_heights(cellx, celly);
  mpreal cx = mpreal(x.toLong() % cell_size) / mpreal(CELL_SIZE);
  mpreal cy = mpreal(y.toLong() % cell_size) / mpreal(CELL_SIZE);
  mpreal a = (heights) & 15;
  mpreal b = (heights >> 4) & 15;
  mpreal d = (heights >> 12) & 15;
  mpreal hx = a - (cx * (a - b));
  mpreal hy = a - (a - (cy * (a - d)));
  return hx - hy;
}

bool World::is_started() const
{
  return this->started;
}
