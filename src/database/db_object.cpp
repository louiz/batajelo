#include "db_object.hpp"
#include "database.hpp"
#include "../logging/logging.hpp"

DbObject::DbObject() {}

DbObject::~DbObject() {}

std::string DbObject::get_class_name()
{
	return "DbObject";
}


void DbObject::set(std::string field, std::string value)
{
  std::map<std::string, std::string>::iterator it = this->values.find(field);
  if (it != this->values.end())
   this->values.erase(it);
  this->values.insert(std::make_pair(field, value));
}

const std::string& DbObject::get(const std::string& field)
{
 // std::map<std::string, std::string>::const_iterator it = this->values.find(field);
  if (this->values.find(field) != this->values.end())
    return this->values[field];
  else
    log_error("The field " << field.c_str() << " is not found");
}

void DbObject::print()
{
  std::map<std::string, std::string>::iterator it;

  for (it = this->values.begin(); it != this->values.end(); it++)
    log_info(it->first << " -> " << it->second);
}


