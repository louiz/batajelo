// User Database Implementation

//cout
#include <iostream>

#include "database.hpp"

#ifndef __USER_HPP__
# define __USER_HPP__

class User : public Database
{
public:
  User();
  User(std::string login, std::string password, std::string email, std::string last_login);
  ~User();
  std::string&Email() const;
  const std::string& getLastLogin() const;
  const std::string& getLogin() const;
  const std::string& getPassword() const;
  void setEmail(const std::string& email);
  void setLastLogin(const std::string& last_login);
  void setLogin(const std::string& login);
  void setPassword(const std::string& password);
  void print();
private:
  std::string login;
  std::string password;
  std::string email;
  std::string last_login;
};

#endif
