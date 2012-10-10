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
  // this->spawn_unit(0, 254, 412);
  // this->spawn_unit(0, 214, 422);
  // this->spawn_unit(0, 334, 402);
  // this->spawn_unit(0, 434, 452);
  // this->spawn_unit(0, 534, 472);
  // this->spawn_unit(0, 634, 482);
  // this->spawn_unit(0, 734, 492);
  // this->spawn_unit(0, 834, 392);
  // this->spawn_unit(0, 734, 392);
  // this->spawn_unit(0, 634, 392);
  // this->spawn_unit(0, 534, 392);
  // this->spawn_unit(0, 434, 392);
  // this->spawn_unit(0, 334, 392);
  // this->spawn_unit(0, 234, 392);
  // this->spawn_unit(0, 334, 792);
  // this->spawn_unit(0, 434, 792);
  // this->spawn_unit(0, 534, 792);
  // this->spawn_unit(0, 634, 792);
  // this->spawn_unit(0, 734, 792);
  // this->spawn_unit(0, 834, 792);
  // this->spawn_unit(0, 934, 792);
  // for (int x = 200; x < 1000; x += 62)
  //   for (int y = 200; y < 1000; y += 62)
  //     this->spawn_unit(0, x, y);
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
  // Action* action = new Action(boost::bind(&World::do_path, this, _1), path_event, this->occupants.size());
  Action* action = new Action(0, path_event, this->occupants.size());
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
  Action* action = new Action(0, doevent, this->occupants.size());
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

