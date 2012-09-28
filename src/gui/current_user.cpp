#include <gui/current_user.hpp>
#include <logging/logging.hpp>

CurrentUser::CurrentUser() {}

CurrentUser::~CurrentUser() {}

const std::map<std::string, std::string>& CurrentUser::get_current_user() const
{
  return this->current_user;
}

const std::string CurrentUser::get_key(const std::string& field) const
{
  std::map<std::string, std::string>::const_iterator it = this->current_user.find(field);
  if (it != this->current_user.end())
    return it->second;
  else
    {
      log_error("The field " << field.c_str() << " in object current_user is not found");
      return "";
    }
}

void CurrentUser::set_key(const std::string& field, const std::string& value)
{
  std::map<std::string, std::string>::const_iterator it = this->current_user.find(field);
  if (it != this->current_user.end())
    this->current_user.erase(it);
  this->current_user.insert(std::make_pair(field, value));
}

const TeamSerial* CurrentUser::get_teams() const
{
  return this->teams;
}

void CurrentUser::set_teams(TeamSerial* teams)
{
  this->teams = teams;
}