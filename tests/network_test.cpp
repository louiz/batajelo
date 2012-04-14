#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/command.hpp>

#include <cstring>

#define BOOST_TEST_MODULE network
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(network_suite1)

BOOST_AUTO_TEST_CASE(command_consistency_test)
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

BOOST_AUTO_TEST_SUITE_END()
