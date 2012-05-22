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

PathEvent::PathEvent(const Command* command)
{
  if (this->from_string(std::string(command->body,
				    command->body_size).c_str()) == false)
    this->valid = false;
  else
    this->valid = true;
}

unsigned long int Event::current_id = 0;