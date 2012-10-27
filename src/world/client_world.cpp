#include <logging/logging.hpp>
#include <world/client_world.hpp>

ClientWorld::ClientWorld(Map* map, Mod& mod):
  World(map, mod)
{
}

ClientWorld::~ClientWorld()
{
}

void ClientWorld::new_occupant_callback(Command* command)
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

void ClientWorld::occupant_left_callback(Command* command)
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

void ClientWorld::new_unit_callback(Command* command)
{
  DoUnitEvent* e = new DoUnitEvent(command);
  if (e->is_valid() == false)
    {
      log_debug("Invalid data for the new entity.");
      return ;
    }
  Action* action = new Action(boost::bind(&World::do_new_unit, this, _1), e);
  log_debug("new entity");
  this->turn_handler->insert_action(action, e->turn);
  if (this->started == true)
    this->confirm_action(e->get_id());
  else
    action->validate_completely();
}

void ClientWorld::handle_start_command(Command* command)
{
  ActionEvent start_event(command);
  if (start_event.is_valid() == false)
    {
      log_warning("Invalid data for START command");
      return ;
    }
  log_debug("The first turn to start is " << start_event.turn);
  this->unpause();
  do
    {
      this->tick(true);
    } while (this->turn_handler->get_current_turn() < start_event.turn - 1);

  this->confirm_turn(start_event.turn);
  this->validate_turn_completely(start_event.turn);
  this->start();
}

void ClientWorld::ok_callback(Command* command)
{
  OkEvent ok_event(command);
  if (ok_event.is_valid() == false)
    {
      log_warning("Invalid data for OK command");
      return ;
    }
  this->completely_validate_action(ok_event.get_id());
}

void ClientWorld::turn_callback(Command* command)
{
  std::istringstream is(std::string(command->body, command->body_size));
  unsigned int number;
  is >> number;
  this->validate_turn_completely(number);
}

void ClientWorld::path_callback(Command* command)
{
  DoMoveEvent* e = new DoMoveEvent(command);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for PATH command");
      return ;
    }
  Action* action = new Action(boost::bind(&World::do_path, this, _1), e);
  this->insert_received_action(action, e);
}

void ClientWorld::build_callback(Command* command)
{
  DoBuildEvent* e = new DoBuildEvent(command);
  log_info("Build_callback: " << e->actor << " " << e->x << ":" << e->y);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for BUILD command");
      return ;
    }
  Action* action = new Action(boost::bind(&World::do_build, this, _1), e);
  this->insert_received_action(action, e);
}

void ClientWorld::insert_received_action(Action* action, ActionEvent* event)
{
  this->turn_handler->insert_action(action, event->turn);
  if (this->started == true)
    this->confirm_action(event->get_id());
  else
    action->validate_completely();
}

void ClientWorld::confirm_action(const unsigned long int id)
{
  Event ok_event(id);
  this->generate_command("OK", ok_event.to_string());
}

void ClientWorld::confirm_initial_turn()
{
  this->confirm_turn(1);
}

void ClientWorld::confirm_turn(const unsigned int number)
{
  std::ostringstream os;
  os << number;
  this->generate_command("T", os.str());
}

void ClientWorld::on_next_turn(unsigned long turn)
{
  if (this->started == true)
    this->confirm_turn(turn+1);
}

bool ClientWorld::action_build(const unsigned int x, const unsigned y, const std::size_t id)
{
  log_info("action_build: " << x << ":" << y << "=" << id);
  assert(this->current_selection.is_empty() == false);
  BuildEvent event;
  event.actor = this->current_selection.get_entities().front()->id;
  Position pos(x, y);
  this->get_cell_at_position(pos, event.x, event.y);
  event.type_id = id;
  this->generate_command("BUILD", event.to_string());
  return true;
}

void ClientWorld::action_spawn(const t_left_click left_click)
{
  assert(this->current_selection.is_empty() == false);
  SpawnEvent event;
  event.actor = this->current_selection.get_entities().front()->id;
  event.type_id = left_click.id;
  this->generate_command("SPAWN", event.to_string());
}

bool ClientWorld::action_move(const unsigned int x, const unsigned y, const std::size_t)
{
  MoveEvent event;
  Entity* entity;
  for (std::list<Entity*>::iterator it = this->entities.begin();
       it != this->entities.end(); ++it)
    {
      log_error(this->entities.size());
      entity = *it;
      if (this->is_entity_selected(entity))
        event.actors_ids.push_back(entity->get_id());
    }
  if (event.actors_ids.size() == 0)
    return false;
  event.x = x;
  event.y = y;
  this->generate_command("MOVE", event.to_string());
  return true;
}

void ClientWorld::select_entity(const Entity* entity)
{
  this->current_selection.add_to_selection(entity);
}

void ClientWorld::unselect_entity(const Entity* entity)
{
  this->current_selection.remove_from_selection(entity);
}

void ClientWorld::clear_selection()
{
  this->current_selection.clear();
}

bool ClientWorld::is_entity_selected(const Entity* entity) const
{
  return this->current_selection.is_in_selection(entity);
}

const Selection& ClientWorld::get_selection() const
{
  return this->current_selection;
}

void ClientWorld::add_selection_change_callback(const t_selection_changed_callback callback)
{
  this->current_selection.on_modified_callbacks.push_back(callback);
}

void ClientWorld::spawn_callback(Command* command)
{
  DoSpawnEvent* e = new DoSpawnEvent(command);
  log_info("Spawn_callback: " << e->actor << " " << e->type_id);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for Spawn command");
      return ;
    }
  Action* action = new Action(boost::bind(&World::do_spawn, this, _1), e);
  this->insert_received_action(action, e);
}
