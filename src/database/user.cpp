#include "user.hpp"

User::User() {}

User::User(std::string login, std::string password, std::string email, std::string last_login):
login(login), password(password), email(email), last_login(last_login) {}

User::~User() {}

const std::string& User::getEmail() const
{
  return this->email;
}

const std::string& User::getLastLogin() const
{
  return this->last_login;
}

const std::string&  User::getLogin() const
{
  return this->login;
}

const std::string& User::getPassword() const
{
  return this->password;
}

void User::setEmail(const std::string& email)
{
  this->email = email;
}

void User::setLastLogin(const std::string& last_login)
{
  this->last_login = last_login;
}


void User::setLogin(const std::string& login)
{
  this->login = login;
}

void User::setPassword(const std::string& Password)
{
  this->password = password;
}

void User::print()
{
  std::cout << "email : " <<  this->getEmail() << std::endl;
  std::cout << "login : " <<  this->getLogin() << std::endl;
  std::cout << "password : " <<  this->getPassword() << std::endl;
  std::cout << "last_login : " <<  this->getLastLogin() << std::endl;
}


int main()
{
  User  a;
  a.queryById("user", "user.login, user.password, user.last_login", 1);
  a.print();
  return 0;
}
