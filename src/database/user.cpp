#include <database/user.hpp>
#include <database/database.hpp>
#include <logging/logging.hpp>

User::User() {}

User::~User() {}

void User::add_achievement(const std::string& achievement_id)
{
  DbObject* user_achievement = new DbObject();
  user_achievement->set("user_id", this->get("id"));
  user_achievement->set("achievement_id", achievement_id);
  if (Database::inst()->update(user_achievement, "user_achievement") != true)
    log_error("Achievement add Failed !");
}

std::vector<DbObject*> User::get_achievements()
{
  std::vector<DbObject*> user_achievements = Database::inst()->get_objects("achievement_id", "user_achievement", "user_id =" + this->get("id"));
  return user_achievements;
}

std::vector<User*> User::get_friends()
{
  std::vector<DbObject*>::iterator objs_it;
  std::vector<User*> friends;
  std::string where = "user.id IN (";
  std::vector<DbObject*> friends_ids = Database::inst()->get_objects("user_friend.friend_id AS id", "user_friend", "user_id = " + this->get("id"));
  if (friends_ids.size() > 0)
    {
      for (objs_it = friends_ids.begin(); objs_it != friends_ids.end(); objs_it++)
	{
	  where += (*objs_it)->get("id");
	  where += (objs_it != friends_ids.end() - 1) ? ", " : ")";
	}
      friends_ids.clear();
      std::vector<DbObject*> friends_obj = Database::inst()->get_objects("user.id, user.login, user.password, user.last_login", "user", where);
      if (friends_obj.size() > 0)
	{
	  for (objs_it = friends_obj.begin(); objs_it != friends_obj.end(); objs_it++)
	    friends.push_back(static_cast<User*>(*objs_it));
	}
      else
	log_warning("No friends Infos found !");
    }
  else
    log_warning("No friends found !");
  return friends;
}

void User::remove_friend(const std::string& friend_id)
{
  DbObject* friendship = new DbObject();
  friendship->set("user_id", this->get("id"));
  friendship->set("friend_id", friend_id);
  if (Database::inst()->update(friendship, "User_Friend") != true)
    log_error("Friendship not found!");
}
