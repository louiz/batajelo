#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/command.hpp>

#include <cstring>

#define BOOST_TEST_MODULE network
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(network_suite1)

BOOST_AUTO_TEST_CASE(command_consistency_test1)
{
  Command* command = new Command;
  command->set_body("coucou les amis");
  command->set_name("FAKE_COMMAND");
  command->pack();

  BOOST_REQUIRE(command->header == std::string("FAKE_COMMAND.15:"));
  BOOST_REQUIRE(command->body_size == 15);
  BOOST_REQUIRE(strncmp(command->body, "coucou les amis", 15) == 0);

  delete command;
}

BOOST_AUTO_TEST_CASE(command_consistency_test2)
{
  Command* command = new Command;
  command->set_body("");
  command->set_name("G");
  command->pack();

  BOOST_REQUIRE(command->header == std::string("G:"));
  BOOST_REQUIRE(command->body_size == 0);
  BOOST_REQUIRE(strncmp(command->body, "", 0) == 0);

  delete command;
}

BOOST_AUTO_TEST_CASE(command_consistency_test3)
{
  Command* command = new Command;
  command->set_body("");
  command->set_name("GUGU");
  command->pack();

  BOOST_REQUIRE(command->header == std::string("GUGU:"));
  BOOST_REQUIRE(command->body_size == 0);
  BOOST_REQUIRE(strncmp(command->body, "", 0) == 0);

  delete command;
}

BOOST_AUTO_TEST_CASE(command_consistency_test4)
{
  Command* command = new Command;
  command->set_body("salut");
  command->set_name("G");
  command->pack();

  BOOST_REQUIRE(command->header == std::string("G5:"));
  BOOST_REQUIRE(command->body_size == 5);
  BOOST_REQUIRE(strncmp(command->body, "salut", 5) == 0);

  delete command;
}

BOOST_AUTO_TEST_CASE(command_copy_test)
{
  Command* command = new Command;
  command->set_body("coucou les amis");
  command->set_name("FAKE_COMMAND");
  command->pack();

  Command* command2 = new Command(*command);
  command2->pack();
  BOOST_REQUIRE(command2->header == std::string("FAKE_COMMAND.15:"));
  BOOST_REQUIRE(command2->body_size == 15);
  BOOST_REQUIRE(strncmp(command2->body, "coucou les amis", 15) == 0);

  delete command;
  delete command2;
}

BOOST_AUTO_TEST_SUITE_END()
