#include <database/db_object.hpp>
#include <database/database.hpp>
#include <database/user.hpp>
#include <database/friend_request.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>

#define BOOST_TEST_MODULE database
#include <boost/test/included/unit_test.hpp>

/* HOW TO USE THE TESTS
  Before launching the tests, you need to execute the data/test.sql script
  It will insert some required values for testing in the database
  Then you are ready to launch the tests !
*/

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

BOOST_AUTO_TEST_CASE(add_friend)
{
  User* user2 = new User();
  user2->set("login", "testing2");
  user2->set("password", "password");
  BOOST_REQUIRE(Database::inst()->update(user2, "User") == true);

  user2 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(user2 != 0);

  User* user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user != 0);

  std::string where = "sender_id=" + user->get("id");
  where += " AND receiver_id = " + user2->get("id");
  where += " AND request_id =";
  where += Config::get("request_friendship", "1").data();
  FriendRequest request;
  request.create(user, user2);
  DbObject* friendship_pending = Database::inst()->get_object("*", "User_Request", where);
  BOOST_REQUIRE(friendship_pending != 0);


  request.accept(user2, user);
  where = "user_id=" + user->get("id");
  where += " AND friend_id=" + user2->get("id");
  DbObject* friendship = Database::inst()->get_object("*", "User_Friend", where);
  BOOST_REQUIRE(friendship != 0);
  BOOST_REQUIRE(friendship->get("user_id") == user->get("id"));
  BOOST_REQUIRE(friendship->get("friend_id") == user2->get("id"));
}

BOOST_AUTO_TEST_CASE(add_user_achievement)
{
  User* user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user != 0);

  user->add_achievement("1");
  std::vector<DbObject*> user_achievements = user->get_achievements();
  BOOST_REQUIRE(user_achievements.size() == 1);
}

BOOST_AUTO_TEST_CASE(add_and_get_replays)
{
  User* user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  User* user2 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(user != 0);
  BOOST_REQUIRE(user2 != 0);

  DbObject* replay = Database::inst()->get_object("id", "Replay", "name='test'");
  BOOST_REQUIRE(replay != 0);

  user->add_replay(replay->get("id"));
  DbObject* user_replay = Database::inst()->get_object("user_id, replay_id", "user_replay", "replay_id=" + replay->get("id") + " AND user_id=" + user->get("id"));
  BOOST_REQUIRE(user_replay != 0);

  user2->add_replay(replay->get("id"));
  DbObject* user_replay2 = Database::inst()->get_object("user_id, replay_id", "user_replay", "replay_id=" + replay->get("id") + " AND user_id=" + user2->get("id"));
  BOOST_REQUIRE(user_replay2 != 0);

  std::vector<DbObject*> replays = user->get_replays();
  BOOST_REQUIRE(replays.size() == 1);

  std::vector<DbObject*> replays2 = user2->get_replays();
  BOOST_REQUIRE(replays.size() == 1);
}

BOOST_AUTO_TEST_CASE(delete_users)
{
  User* user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  User* user2 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(Database::inst()->remove(user, "User") == true);
  BOOST_REQUIRE(Database::inst()->remove(user2, "User") == true);

  user = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user == 0);

  user2 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user2 == 0);
}

BOOST_AUTO_TEST_SUITE_END()
