#include "user.hpp"

User::User() {}

User::User(std::string id): _id(id)
{
  // FUCKING POINTER Y U NO WORK ?
  this->ftab["email"] = &email;
  // get_class_name for first param ?
  std::string table = "user";
  std::string fields = "user.login, user.password, user.last_login, user_setting.email";
  std::string join = "INNER JOIN user_setting ON user_setting.user_id = user.id";
  this->query_by_id(table, fields, join, this->_id);

  std::vector<std::pair<int, map_row> >::iterator vector_it;
  map_row::iterator map_it;

  for (vector_it = this->results.begin(); vector_it != this->results.end(); vector_it++)
  {
      map_row row = vector_it->second;
      for (map_it = row.begin(); map_it != row.end(); map_it++)
      {
       map_it->first = map_it->second;
        *(this->ftab[map_it->first])()
//        std::cout << map_it->first << " -> " << map_it->second << std::endl;
      }
  }
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

int main()
{
  std::string id = "1";
  User user(id);
 // user.print();
   return 0;
}
