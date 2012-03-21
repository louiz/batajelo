#include "db_object.hpp"
#include "database.hpp"
#include "../logging/logging.hpp"

DbObject::DbObject() {}

DbObject::~DbObject() {}

void DbObject::set(std::string field, std::string value)
{
  std::map<std::string, std::string>::iterator it = this->values.find(field);
  if (it != this->values.end())
   this->values.erase(it);
  this->values.insert(std::make_pair(field, value));
}

const std::string DbObject::get(const std::string& field) const
{
  std::map<std::string, std::string>::const_iterator it = this->values.find(field);
  if (it != this->values.end())
    return it->second;
  else
    {
      log_error("The field " << field.c_str() << " is not found");
      return "";
    }
}

void DbObject::print()
{
  std::map<std::string, std::string>::iterator it;
	std::cout << this->values.size();

  for (it = this->values.begin(); it != this->values.end(); it++)
    log_info(it->first << " -> " << it->second);
}

template<typename T> void DbObject::print_vector(std::vector<T*> objs)
{
	// WTF, doesnt work :(
/*	std::vector<T::template>::iterator t_it;

  for (t_it = objs.begin(); it != objs.end(); t_it++)
		(*t_it).print();*/
}

