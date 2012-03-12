// User Database Implementation

//cout
#include <iostream>

#include "database.hpp"

#ifndef __USER_HPP__
# define __USER_HPP__

typedef std::string& (*fptr) ();

class User : public Database
{
public:
  User();
  User(std::string _id);
  ~User();

  std::string& email(void);
  std::string& last_login(void);
  std::string& login(void);
  std::string& id(void);
  std::string& password(void);
private:
  fptr ftab[1];
  std::string _email;
  std::string _last_login;
  std::string _login;
  std::string _id;
  std::string _password;
};

#endif
