#include <database/db_object.hpp>
#include <database/database.hpp>
#include <database/user.hpp>
#include <database/friend_request.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>
#include <sha.h>
#include <base64.h>

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
  BOOST_REQUIRE(user1 == nullptr);
  delete user1;
}

BOOST_AUTO_TEST_CASE(update_user)
{
  std::string digest;
  CryptoPP::SHA256 hash;

  CryptoPP::StringSource("nightmare", true,
   new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

  User* user = new User();
  user->set("login", "testing");
  user->set("password", digest);
  BOOST_REQUIRE(Database::inst()->update(user, "User") == true);
  delete user;

  User* user1 = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user1 != 0);
  BOOST_REQUIRE(user1->get("password") == digest);

  digest.clear();
  CryptoPP::StringSource("nightmare2", true,
   new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));
  user1->set("password", digest);
  BOOST_REQUIRE(Database::inst()->update(user1, "User") == true);

  User* user2 = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user2 != 0);
  BOOST_REQUIRE(user2->get("password") == digest);
}

BOOST_AUTO_TEST_CASE(add_friend)
{
  std::string digest;
  CryptoPP::SHA256 hash;

  CryptoPP::StringSource("lol", true,
   new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

  User* user2 = new User();
  user2->set("login", "testing2");
  user2->set("password", digest);
  BOOST_REQUIRE(Database::inst()->update(user2, "User") == true);

  user2 = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(user2 != 0);

  User* user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
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
  User* user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user != 0);

  user->add_achievement("1");
  std::vector<DbObject*> user_achievements = user->get_achievements();
  BOOST_REQUIRE(user_achievements.size() == 1);
}

BOOST_AUTO_TEST_CASE(add_and_get_replays)
{
  User* user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  User* user2 = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
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

BOOST_AUTO_TEST_CASE(add_update_remove_ban)
{
  User* user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user);
  user->add_ban(24);
  DbObject* user_infos = Database::inst()->get_object("*", "user", "id=" + user->get("id"));
  BOOST_REQUIRE(user_infos->get_int("is_banned") == 1);
  user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));

  BOOST_REQUIRE(user);
  DbObject* user_ban = Database::inst()->get_object("*", "user_ban", "user_id=" + user->get("id"));
  boost::posix_time::ptime last_unban_time(boost::posix_time::time_from_string(user_ban->get("ban_end")));
  BOOST_REQUIRE(user_ban != 0);

  user->update_ban(24);
  user_ban = Database::inst()->get_object("*", "user_ban", "user_id=" + user->get("id"));
  boost::posix_time::ptime unban_time(boost::posix_time::time_from_string(user_ban->get("ban_end")));
  BOOST_REQUIRE(last_unban_time + boost::posix_time::hours(24) == unban_time);

  user->remove_ban();
  user_infos = Database::inst()->get_object("*", "user", "id=" + user->get("id"));
  BOOST_REQUIRE(user_infos->get_int("is_banned") == 0);
}

BOOST_AUTO_TEST_CASE(delete_users)
{
  User* user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  User* user2 = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing2'"));
  BOOST_REQUIRE(user);
  BOOST_REQUIRE(user2);
  BOOST_REQUIRE(Database::inst()->remove(user, "User") == true);
  BOOST_REQUIRE(Database::inst()->remove(user2, "User") == true);

  user = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user == 0);

  user2 = dynamic_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user2 == 0);
}

BOOST_AUTO_TEST_SUITE_END()
