#include <world/server_world.hpp>

ServerWorld::ServerWorld():
  World()
{
  this->init();
}

ServerWorld::~ServerWorld()
{
}

void ServerWorld::init()
{
  log_debug("Creating initial world state");
  // Spawn initial units.
  Entity* new_entity = this->create_entity(0);
  new_entity->y = rand() % 100 + 450;
  new_entity->x = rand() % 100 + 450;

  this->insert_entity(new_entity);
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
  path_event->turn = current_turn + 3;
  this->generate_command("PATH", path_event->to_string());
  Action* action = new Action(boost::bind(&World::do_path, this, _1), path_event, this->occupants.size());
  this->turn_handler->insert_action(action, path_event->turn);
}
