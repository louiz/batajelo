#include <database/user.hpp>
#include <database/database.hpp>
#include <logging/logging.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <sstream>

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

void User::add_replay(const std::string& replay_id)
{
  DbObject* user_replay = new DbObject();
  user_replay->set("replay_id", replay_id);
  user_replay->set("user_id", this->get("id"));
  if (Database::inst()->update(user_replay, "user_replay") != true)
    log_error("User_Replay add Failed !");
}

std::vector<DbObject*> User::get_replays()
{
  const std::string where = "user_replay INNER JOIN replay ON user_replay.replay_id = replay.id";
  std::vector<DbObject*> replays = Database::inst()->get_objects("replay_id, name", where, "user_id=" + this->get("id"));
  if (replays.size() == 0)
    {
	    log_warning("No Replays found !");
    }
  return replays;
}

void User::add_ban(const long hours)
{
  if (this->get_int("is_banned") == 1)
    {
      log_error("User is already banned ! ")
    }
  else
    {
      boost::posix_time::ptime current_time = boost::posix_time::second_clock::local_time();
      boost::posix_time::ptime unban_time = current_time + boost::posix_time::hours(hours);
      std::string ban_start = to_iso_extended_string(current_time).replace(10, 1, " ");
      std::string ban_end = to_iso_extended_string(unban_time).replace(10, 1, " ");

      DbObject* ban = new DbObject();
      ban->set("user_id", this->get("id"));
      ban->set("ban_start", ban_start);
      ban->set("ban_end", ban_end);
      if (Database::inst()->update(ban, "user_ban") != true)
        {
          log_error("Couldn't insert the ban.");
        }

      DbObject* user = new DbObject();
      user->set("is_banned", "1");
      user->set("id", this->get("id"));
      if (Database::inst()->update(user, "user") != true)
        {
          log_error("Couldn't update the user.");
        }
    }
}

void User::update_ban(const long hours)
{
  if (this->get_int("is_banned") == 0)
    {
      log_error("User is not banned ! ")
    }
  else
    {
      DbObject* user_ban = Database::inst()->get_object("id, user_id, ban_start, ban_end", "user_ban", "user_id=" + this->get("id"));
      boost::posix_time::ptime last_unban_time(boost::posix_time::time_from_string(user_ban->get("ban_end")));
      boost::posix_time::ptime unban_time = last_unban_time + boost::posix_time::hours(hours);
      boost::posix_time::ptime current_time = boost::posix_time::second_clock::local_time();

      if (unban_time < current_time)
        {
          log_error("The new ban time is already passed, use delete_ban to remove the ban !")
        }
      else
        {
          std::string ban_end = to_iso_extended_string(unban_time).replace(10, 1, " ");
          user_ban->set("ban_end", ban_end);
          if (Database::inst()->update(user_ban, "user_ban") != true)
          {
            log_error("Couldn't update the ban.")
          }
        }

    }
}

void User::remove_ban()
{
  if (this->get_int("is_banned") == 0)
    {
      log_error("User is not banned.")
    }
  else
    {
      DbObject* user = new DbObject();
      user->set("id", this->get("id"));
      user->set("is_banned", "0");
      if (Database::inst()->update(user, "user") != true)
        {
          log_error("Couldn't update the user.")
        }
    }
}
