#include <network/command.hpp>

Command::Command():
  body(0),
  body_size(0),
  callback(0)
{
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
  std::ostringstream slength;
  slength << this->body_size;
  this->header = std::string(this->name) + "." + slength.str() + ":";
}
