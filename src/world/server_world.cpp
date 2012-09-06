#include <world/server_world.hpp>

ServerWorld::ServerWorld():
  World()
{
  this->init();
}

ServerWorld::~ServerWorld()
{
}

void ServerWorld::spawn_unit(const size_t type, const int x, const int y)
{
  Entity* new_entity = this->create_entity(type);
  new_entity->pos = Position(x, y);
  EntityEvent* e = new EntityEvent(new_entity);
  e->turn = 1;
  Action* action = new Action(0, e, 0);
  action->validate_completely();
  this->replay->insert_action(action);
}

void ServerWorld::init()
{
  log_error("Creating initial world state");
  this->spawn_unit(0, 353, 325);
  this->spawn_unit(0, 154, 512);
  this->spawn_unit(0, 254, 512);
  this->spawn_unit(0, 154, 312);
  log_debug("done");
}

void ServerWorld::move_callback(Command* command)
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
  path_event->turn = current_turn + 2;
  this->generate_command("PATH", path_event->to_string());
  // Action* action = new Action(boost::bind(&World::do_path, this, _1), path_event, this->occupants.size());
  Action* action = new Action(0, path_event, this->occupants.size());
  this->turn_handler->insert_action(action, path_event->turn);
}

bool ServerWorld::validate_action(const unsigned int id, const unsigned long int by)
{
  log_debug("Action " << id << " validated by " << by);
  bool res = this->turn_handler->validate_action(id, by);
  log_debug("This made it completely validated:" << res);
  return res;
}

bool ServerWorld::validate_turn(const unsigned int id, const unsigned long int by)
{
  return this->turn_handler->validate_turn(id, by, this->occupants.size());
}

