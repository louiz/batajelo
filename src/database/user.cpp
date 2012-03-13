#include "user.hpp"

User::User() {}

User::User(std::string id): _id(id)
{
  // pointeur sur fonction :(
  typedef std::string& (User::*fptr)();
  typedef std::map<std::string, fptr> m_fptr;
  m_fptr m_fields;
  // add a pointer on function in map
  m_fields.insert(m_fptr::value_type("email", &User::email));
  // create iterator
  m_fptr::const_iterator m_fields_it;

//  boost::function<std::string&()>email = boost::bind(&User::email, this);
  std::string table = "user";
  std::string fields = "user.login, user.password, user.last_login, user_setting.email";
  std::string join = "INNER JOIN user_setting ON user_setting.user_id = user.id";
  std::vector< std::pair<int, map_row> >::iterator vector_it;
  map_row::iterator map_it;
  map_row row;

  this->query_by_id(table, fields, join, this->_id);
  for (vector_it = this->results.begin(); vector_it != this->results.end(); vector_it++)
  {
      row = vector_it->second;
      for (map_it = row.begin(); map_it != row.end(); map_it++)
      {
        // get the pointer on function for this result
        m_fields_it = m_fields.find(map_it->first);
        // call the setter but doesnt work :(
        // error: must use '.*' or '->*' to call pointer-to-member function
        m_fields_it->second() = map_it->second;
      }

  //      this->(map_it->first)() = map_it->second;
  }
}

User::~User() {}

std::string& User::email()
{
  return this->_email;
}

std::string& User::last_login()
{
  return this->_last_login;
}

std::string& User::login()
{
  return this->_login;
}

std::string& User::password()
{
  return this->_password;
}

void User::print()
{
  std::cout << "Hi !" << std::endl;
  std::cout << "My login is : " << this->login() << std::endl;;
  std::cout << "Last time has logged in is : " << this->last_login() << std::endl;;
  std::cout << "My password is : " << this->password() << std::endl;;
  std::cout << "My email is : " << this->email() << std::endl;;
  std::cout << "Bisosu !" << std::endl;
}
int main()
{
  std::string id = "1";
  User user(id);
  user.print();
  return 0;
}
