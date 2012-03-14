#include "db_object.hpp"
#include "database.hpp"

DbObject::DbObject() {}

DbObject::~DbObject() {}

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
    printf("The field %s is not found", field.c_str());
}

void DbObject::print()
{
  std::map<std::string, std::string>::iterator it;

  for (it = this->values.begin(); it != this->values.end(); it++)
    std::cout << it->first << " -> " << it->second << std::endl;
}

int main()
{
  Database db;
  DbObject user = db.get_object_by_id("user", "1");
  std::cout << user.get("login") << std::endl;
  user.set("login", "louiz@louiz.org");
  std::cout << user.get("login") << std::endl;
//  user.save_to_db();
  return 0;
}
