#include <boost/bind.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <game/event.hpp>
#include <world/world.hpp>

World::World():
  started(false)
{
  this->entities_iterator = this->entities.begin();
  this->init();
  this->turn_handler = new TurnHandler;
}

World::~World()
{
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
      if (event.actors_ids.size() == 0)
	return ;
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

void World::new_occupant_callback(Command* command)
{
  log_debug("new_occupant_callback");
  Occupant* occupant = new Occupant;

  std::string data(command->body, command->body_size);
  if (occupant->from_string(data) == false)
    {
      log_error("Invalid data received for the occupant.");
      return ;
    }
  log_debug("Occupant: " << occupant->name << " " << occupant->number);
  this->add_new_occupant(occupant);
}

void World::handle_start_command(Command* command)
{
  // Action* action = new Action(0, event, this->occupants.size());
  // this->turn_handler->insert_action(action, turn);
  Event* start_event = new Event(command);
  if (start_event->is_valid() == false)
    {
      log_warning("Invalid data for START command");
      return ;
    }
  this->install_start_action(start_event, 1);
}

void World::occupant_left_callback(Command* command)
{
  Occupant occupant;
  if (occupant.from_string(std::string(command->body, command->body_size)) == false)
    {
      log_error("Invalid data received for the leaving occupant.");
      return ;
    }
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
  this->occupants.push_back(occupant);
  log_debug("Adding new occupant to the world:" << occupant->name << " " << occupant->number);
}

void World::new_entity_callback(Command* command)
{
  Entity* new_entity = new Entity;
  std::string data(command->body, command->body_size);
  log_debug("New_Entity: " << data);
  if (new_entity->from_string(data) == false)
    {
      log_debug("Invalid data for the new entity.");
      return ;
    }
  this->insert_entity(new_entity);
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

void World::install_start_action(Event* event, unsigned int turn)
{
  Action* action = new Action(0, event, this->occupants.size());
  this->turn_handler->insert_action(action, turn);
}

void World::tick()
{
  if (this->started == false)
    return ;
  this->turn_handler->tick();
  Entity* entity;
  this->reset_entity_iterator();
  while ((entity = this->get_next_entity()))
    {
      entity->tick();
    }
}

void World::start()
{
  log_debug("start");
  if (this->started == true)
    return;
  this->started = true;
}

void World::confirm_initial_turn()
{
  Turn* turn = this->turn_handler->get_turn(1);

  turn->reset_action_iterator();
  Action* action;

  Event* ok_event;
  while ((action = turn->get_next_action()))
    {
      if (action->is_completely_validated() == false)
	{
	  ok_event = new Event(action->get_id());
	  this->generate_command("OK", ok_event->to_string());
	  delete ok_event;
	}
    }
}

void World::ok_callback(Command* command)
{
  OkEvent ok_event(command);
  if (ok_event.is_valid() == false)
    {
      log_warning("Invalid data for OK command");
      return ;
    }
  this->validate_action(ok_event.get_id(), ok_event.client_id);
}

void World::move_callback(Command* command)
{
  // TODO, do an actuall path finding, and other stuff, and
  // generate a PathEvent, instead of a MoveEvent.
  MoveEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for MOVE command");
      return ;
    }
  PathEvent* path_event = new PathEvent(event);
  unsigned long current_turn = this->turn_handler->get_current_turn();
  log_debug("Currently at: " << current_turn);
  log_debug("move_callback: " << path_event->to_string());
  path_event->turn = current_turn + 3;
  this->generate_command("PATH", path_event->to_string());
  Action* action = new Action(boost::bind(&World::do_path, this, _1), path_event, this->occupants.size());
  this->turn_handler->insert_action(action, path_event->turn);
}

void World::path_callback(Command* command)
{
  PathEvent* e = new PathEvent(command);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for PATH command");
      return ;
    }
  log_debug("Must move unit " << e->actors_ids[0] << " to " << e->x << ":" << e->y << " on turn " << e->turn);
  Action* action = new Action(boost::bind(&World::do_path, this, _1), e, this->occupants.size());
  this->turn_handler->insert_action(action, e->turn);
  this->confirm_action(e->get_id());
}

void World::confirm_action(const unsigned long int id)
{
  Event ok_event(id);
  this->generate_command("OK", ok_event.to_string());
}

void World::do_path(Event* event)
{
  PathEvent* path_event = static_cast<PathEvent*>(event);
  log_debug("DOING PATH");
  Path path(path_event->x, path_event->y);
  unsigned short entity_id = path_event->actors_ids[0];
  Entity* entity = this->get_entity_by_id(entity_id);
  entity->set_path(path);
}

void World::validate_action(const unsigned int id, const unsigned long int by)
{
  log_debug("Action " << id << " validated by " << by);
  bool ret = this->turn_handler->validate_action(id, by);
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
