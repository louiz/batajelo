#include <world/client_world.hpp>

ClientWorld::ClientWorld():
  World()
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

void ClientWorld::new_entity_callback(Command* command)
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

void ClientWorld::handle_start_command(Command* command)
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
  PathEvent* e = new PathEvent(command);
  if (e->is_valid() == false)
    {
      log_warning("Invalid data for PATH command");
      return ;
    }
  log_debug("Must move unit " << e->actors_ids[0] << " to " << e->x << ":" << e->y << " on turn " << e->turn);
  Action* action = new Action(boost::bind(&World::do_path, this, _1), e, this->occupants.size());
  this->turn_handler->insert_action(action, e->turn);
  if (this->started == true)
    this->confirm_action(e->get_id());
  else
    this->advance_replay_until_paused();
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
  this->confirm_turn(turn+1);
}

void ClientWorld::handle_event(actions::Type type, unsigned int x, unsigned y)
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
