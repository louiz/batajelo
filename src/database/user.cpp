#include "user.hpp"
#include "database.hpp"
#include "../logging/logging.hpp"

User::User() {}

User::~User() {}

std::string User::get_class_name()
{
	return "User";
}


std::vector<User*> User::get_friends()
{
	std::vector<DbObject*>::iterator objs_it;
	std::vector<User*> friends;
	std::string where = "user.id IN (";
	std::vector<DbObject*> friends_ids = Database::inst()->get_objects_by_id("user_friend.friend_id AS id", "user_friend", "user_id = " + this->get("id"));
	if (friends_ids.size() > 0)
	{
		for (objs_it = friends_ids.begin(); objs_it != friends_ids.end(); objs_it++)
		{
			where += (*objs_it)->get("id");
			where += (objs_it != friends_ids.end() - 1) ? ", " : ")";
		}
		friends_ids.clear();
		std::vector<DbObject*> friends_obj = Database::inst()->get_objects_by_id("user.id, user.login, user.password, user.last_login", "user", where);
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
