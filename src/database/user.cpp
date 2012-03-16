#include "user.hpp"
#include "database.hpp"
#include "../logging/logging.hpp"

User::User() {}

User::~User() {}

std::string User::get_class_name()
{
	return "User";
}


std::vector<User> User::get_friends()
{
	std::vector<DbObject*>::iterator obj;
	std::string where = "WHERE user.id IN (";
	std::vector<User> friends;
	std::vector<DbObject*> friends_ids = Database::inst()->get_objects_by_id("user_friend.friend_id AS id", "user_friend", "user_id = " + this->get("id"));
		std::cout << "allo";
	if (friends_ids.size() > 0)
	{
		for (obj = friends_ids.begin(); obj != friends_ids.end(); obj++)
		{
			std::cout << "cc";
			std::cout << (*obj)->get("id");
			where += (*obj)->get("id");
			where += (obj != friends_ids.end() - 1) ? ", " : ")";
		}
		// boucler sur le vector pour dynamic_cast ??
//		std::vector<User*> friends = dynamic_cast<User*>(&Database::inst()->get_objects_by_id("user.id, user.login", "user", where));
	} else
		log_warning("No friends found !");
	return friends;
}
