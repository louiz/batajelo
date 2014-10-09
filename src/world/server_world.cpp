#include <logging/logging.hpp>
#include <world/server_world.hpp>

ServerWorld::ServerWorld(Mod& mod):
  World(mod)
{
  this->init();
}

ServerWorld::~ServerWorld()
{
}

void ServerWorld::spawn_unit(const size_t type, const int x, const int y)
{
  Unit* new_unit = this->create_unit(type);
  new_unit->pos = Position(x, y);
  DoUnitEvent* e = new DoUnitEvent(new_unit);
  e->turn = 1;
  Action* action = new Action(nullptr, e, 0);
  action->validate_completely();
  this->replay->insert_action(action);
}

void ServerWorld::init()
{
  log_error("Creating initial world state");
  this->spawn_unit(0, 300, 300);
  // this->spawn_unit(0, 0, 400);
  // this->spawn_unit(0, 0, 600);

  // this->spawn_unit(0, 200, 0);
  // this->spawn_unit(0, 400, 0);
  // this->spawn_unit(0, 600, 0);

  // this->spawn_unit(0, 200, 200);
  // this->spawn_unit(0, 400, 400);
  // this->spawn_unit(0, 600, 600);
  log_debug("done");
}

void ServerWorld::move_callback(Command* command)
{
  MoveEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for MOVE command");
      return ;
    }
  DoMoveEvent* path_event = new DoMoveEvent(event);
  unsigned long current_turn = this->turn_handler->get_current_turn();
  log_debug("Currently at: " << current_turn);
  log_debug("move_callback: " << path_event->to_string());
  path_event->turn = current_turn + 2;
  this->generate_command("PATH", path_event->to_string());
  // Action* action = new Action(std::bind(&World::do_path, this, std::placeholders::_1), path_event, this->occupants.size());
  Action* action = new Action(nullptr, path_event, this->occupants.size());
  this->turn_handler->insert_action(action, path_event->turn);
}

void ServerWorld::build_callback(Command* command)
{
  BuildEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for BUILD command");
      return ;
    }
  // Here check for command validity etc etc.
  // if the command is valid, just return a DoBuildEvent with the same value
  DoBuildEvent* doevent = new DoBuildEvent(event);
  doevent->turn = this->turn_handler->get_current_turn() + 2;
  this->generate_command("BUILD", doevent->to_string());
  Action* action = new Action(nullptr, doevent, this->occupants.size());
  this->turn_handler->insert_action(action, doevent->turn);
}

void ServerWorld::spawn_callback(Command* command)
{
  SpawnEvent event(command);
  if (event.is_valid() == false)
    {
      log_warning("Invalid data for SPAWN command");
      return ;
    }
  DoSpawnEvent* doevent = new DoSpawnEvent(event);
  doevent->turn = this->turn_handler->get_current_turn() + 2;
  this->generate_command("SPAWN", doevent->to_string());
  Action* action = new Action(nullptr, doevent, this->occupants.size());
  this->turn_handler->insert_action(action, doevent->turn);
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

