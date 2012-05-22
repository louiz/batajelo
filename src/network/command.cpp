#include <network/command.hpp>

Command::Command():
  body(0),
  body_size(0),
  callback(0)
{
}

Command::Command(const Command& command)
{
  this->callback = command.callback;
  this->header = command.header;
  this->name = command.name;
  this->set_body(command.body, command.body_size);
}

Command::~Command()
{
  delete[] this->body;
}

void Command::set_body(const char* body, int size)
{
  if (size == -1)
    size = ::strlen(body);
  this->body = new char[size];
  ::memcpy(this->body, body, size);
  this->set_body_size(size);
}

void Command::set_name(const std::string name)
{
  this->name = name;
}

void Command::set_body_size(int size)
{
  this->body_size = size;
}

void Command::pack()
{
  assert(this->name.size() > 0);

  std::ostringstream slength;
  slength << this->body_size;
  // . separator is ommited if name is 1 char long
  if ((this->name.size() == 1))
    this->header = std::string(this->name);
  else
    this->header = std::string(this->name) + ".";
  if (this->body_size > 0) // body size is ommited if it's 0
    this->header += slength.str() + ":";
  else
    this->header += ":";
}
