#include <network/message.hpp>
#include <assert.h>

Message::Message():
  body(nullptr),
  body_size(0),
  callback(nullptr)
{
}

Message::Message(const Message& message):
  callback(message.callback),
  header(message.header),
  name(message.name)
{
  this->set_body(message.body, message.body_size);
}

Message::~Message()
{
  delete[] this->body;
}

void Message::set_body(const char* body, int size)
{
  if (size == -1)
    size = ::strlen(body);
  this->body = new char[size];
  ::memcpy(this->body, body, size);
  this->set_body_size(size);
}

void Message::set_body(const google::protobuf::Message& msg)
{
  const int size = msg.ByteSize();
  this->body = new char[size];
  this->set_body_size(size);
  const auto&& res = msg.SerializeToArray(this->body, size);
  log_debug("Setting message body with protobuf: " << msg.ShortDebugString());
  assert(res);
}

void Message::set_name(const std::string& name)
{
  this->name = name;
}

void Message::set_body_size(int size)
{
  this->body_size = size;
}

void Message::pack()
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
