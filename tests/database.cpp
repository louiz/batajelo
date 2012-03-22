#include <database/db_object.hpp>
#include <database/database.hpp>
#include <database/user.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>

#define DEFAULT_CONF_PATH ../src/config/batajelo.conf


#define BOOST_TEST_MODULE database
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(database_suite1)

BOOST_AUTO_TEST_CASE(database_test_1)
{
  BOOST_TEST_MESSAGE("Testing 0 row result");
  DbObject* user1 = Database::inst()->get_object("*", "User", "id=1");
  BOOST_REQUIRE(user1 == 0);
}

BOOST_AUTO_TEST_CASE(database_test_2)
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
  delete user1;

  user1 = static_cast<User*>(Database::inst()->get_object("*", "User", "login='testing'"));
  BOOST_REQUIRE(user1 != 0);
  BOOST_REQUIRE(user1->get("password") == "new_pass");
}

BOOST_AUTO_TEST_SUITE_END()
