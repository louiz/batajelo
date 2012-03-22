#include "../database/database.hpp"
#include "../database/user.hpp"
#include "../config/config.hpp"

#include "../logging/logging.hpp"
// not sure this include is needed ?
#include <typeinfo>

int main(int argc, char *argv[])
{
 (void)argc;
 (void)argv;
	Config::read_conf("../config/database.conf");

	User* user = static_cast<User*>(Database::inst()->get_object("user.id, user.login, user.password, user.last_login", "user", "user.id = 1"));
	std::vector<User*> friends = user->get_friends();
	std::vector<User*>::iterator users_it;
	if (friends.size() > 0)
	{
		for (users_it = friends.begin(); users_it != friends.end(); users_it++)
			(*users_it)->print();
	}
	return 0;
}
