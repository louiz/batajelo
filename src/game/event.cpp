#include <game/event.hpp>
#include <string>

Event::Event(const unsigned long int id):
  id(id),
  valid(true)
{
}

Event::Event(const Command* command)
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

Event::Event():
  id(Event::current_id++),
  valid(true)
{
}


OkEvent::OkEvent(const Command* command)
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

MoveEvent::MoveEvent(const Command* command)
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

ActionEvent::ActionEvent(const Command* command):
  name("")
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;

}

DoMoveEvent::DoMoveEvent(const Command* command):
  ActionEvent("PATH")
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

DoUnitEvent::DoUnitEvent(const Command* command):
  ActionEvent("NEW_UNIT")
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

BuildEvent::BuildEvent(const Command* command)
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

DoBuildEvent::DoBuildEvent(const Command* command):
  ActionEvent("BUILD")
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

SpawnEvent::SpawnEvent(const Command* command)
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

DoSpawnEvent::DoSpawnEvent(const Command* command):
  ActionEvent("SPAWN")
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

unsigned long int Event::current_id = 0;
