#include <gui/current_user.hpp>
#include <logging/logging.hpp>

CurrentUser::CurrentUser()
{}

CurrentUser::~CurrentUser()
{}

const auto& CurrentUser::get_current_user() const
{
  return this->current_user;
}

const std::string CurrentUser::get_key(const std::string& field) const
{
  const auto it = this->current_user.find(field);
  if (it != this->current_user.end())
    return it->second;
  else
    return {};
}

void CurrentUser::set_key(const std::string& field, const std::string& value)
{
  this->current_user[field] = value;
}
