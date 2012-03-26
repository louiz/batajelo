#include <database/db_object.hpp>
#include <database/database.hpp>
#include <database/user.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>

#define BOOST_TEST_MODULE database
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_suite1)

BOOST_AUTO_TEST_CASE(empty_user)
{
  BOOST_TEST_MESSAGE("Testing 0 row result");
  DbObject* user1 = Database::inst()->get_object("*", "User", "id=1");
  BOOST_REQUIRE(user1 == 0);
  delete user1;
}

BOOST_AUTO_TEST_CASE(update_user)
{
  User* user = new User();
  user->set("login", "testing");
  user->set("password", "password");
  BOOST_REQUIRE(Database::inst()->update(user, "User") == true);
  delete user;

  User* user1 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user1 != 0);
  BOOST_REQUIRE(user1->get("password") == "password");

  user1->set("password", "new_pass");
  BOOST_REQUIRE(Database::inst()->update(user1, "User") == true);

  User* user2 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user2 != 0);
  BOOST_REQUIRE(user2->get("password") == "new_pass");
}

BOOST_AUTO_TEST_CASE(create_friend)
{
  User* user2 = new User();
  user2->set("login", "testing2");
  user2->set("password", "password");
  BOOST_REQUIRE(Database::inst()->update(user2, "User") == true);

  user2 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(user2 != 0);

  User* user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user != 0);

  user->add_friend(user2->get("id"));
  DbObject* friendship = Database::inst()->get_object("*", "User_Friend", "user_id='" + user->get("id") + "' AND friend_id = '" + user2->get("id") + "'" );
  BOOST_REQUIRE(friendship != 0);
  BOOST_REQUIRE(friendship->get("user_id") == user->get("id"));
  BOOST_REQUIRE(friendship->get("friend_id") == user2->get("id"));
}

BOOST_AUTO_TEST_CASE(create_user_achievement)
{
  User* user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user != 0);

  DbObject* achievement = new DbObject();
  achievement->set("name", "first");
  achievement->set("difficulty", "EASY");
  achievement->set("description", "lol");
  BOOST_REQUIRE(Database::inst()->update(achievement, "Achievement") == true);

  user->add_achievement("1");
  std::vector<DbObject*> user_achievements = user->get_achievements();
  BOOST_REQUIRE(user_achievements.size() == 1);
}

BOOST_AUTO_TEST_CASE(delete_users)
{
  BOOST_REQUIRE(Database::inst()->remove("User", "login='testing2' OR login='testing'") == true);

  User* user;
  user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user == 0);

  user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(user == 0);
}

BOOST_AUTO_TEST_SUITE_END()
