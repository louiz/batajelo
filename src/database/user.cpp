#include "user.hpp"

User::User() {}

User::User(std::string id): _id(id)
{
  // get_class_name for first param ?
  std::string table = "user";
  std::string fields = "user.login, user.password, user.last_login, user_setting.email";
  std::string join = "INNER JOIN user_setting ON user_setting.user_id = user.id";
  this->query_by_id(table, fields, join, this->_id);
}

User::~User() {}

std::string& User::email(void)
{
  return this->_email;
}

std::string& User::last_login(void)
{
  return this->_last_login;
}

std::string& User::login(void)
{
  return this->_login;
}

std::string& User::password(void)
{
  return this->_password;
}

void User::print()
{
  std::cout << "email : " <<  this->email() << std::endl;
  std::cout << "login : " <<  this->login() << std::endl;
  std::cout << "password : " <<  this->password() << std::endl;
  std::cout << "last_login : " <<  this->last_login() << std::endl;
}

int main()
{
  std::string id = "1";
  User user(id);
 // user.print();
  return 0;
}
