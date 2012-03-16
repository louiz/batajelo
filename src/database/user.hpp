// User Implementation

#include <vector>

#include "db_object.hpp"

#ifndef __DB_USER_HPP__
# define __DB_USER_HPP__

class User : public DbObject
{
public:
  User();
  ~User();
  std::vector<User> get_friends();
	virtual std::string get_class_name();

  std::map<std::string, std::string> values;
};

#endif
