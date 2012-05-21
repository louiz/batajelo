#include <game/event.hpp>
#include <string>

Event::Event(const unsigned long int id):
  id(id)
{
}

Event::Event(const Command* command)
{
  this->from_string(std::string(command->body,
				command->body_size).c_str());
}

Event::Event():
  id(Event::current_id++)
{
}


OkEvent::OkEvent(const Command* command)
{
  this->from_string(std::string(command->body,
				command->body_size).c_str());
}

MoveEvent::MoveEvent(const Command* command)
{
  this->from_string(std::string(command->body,
				command->body_size).c_str());
}

PathEvent::PathEvent(const Command* command)
{
  this->from_string(std::string(command->body,
				command->body_size).c_str());
}

unsigned long int Event::current_id = 0;
