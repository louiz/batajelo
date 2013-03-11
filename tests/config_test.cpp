#include <config/config.hpp>
#include <logging/logging.hpp>
#include <iostream>
#include <fstream>

#define BOOST_TEST_MODULE config
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(config_suite1)

static int a;

void on_conf_changed()
{
  ++a;
}

BOOST_AUTO_TEST_CASE(config_test1)
{
  std::ofstream os("tests/test.conf");
  os << "coucou=machin" << std::endl;
  os << "number=2" << std::endl;
  os << "empty=" << std::endl;
  os.close();

  a = 0;

  BOOST_REQUIRE(Config::read_conf("tests/test.conf") == true);
  Config::connect(&on_conf_changed);
  BOOST_REQUIRE(Config::get("coucou", "") == "machin");
  BOOST_REQUIRE(Config::get("does not exist", "zizi") == "zizi");
  BOOST_REQUIRE(Config::get_int("number", 0) == 2);
  BOOST_REQUIRE(Config::get_int("does not exist", 1) == 1);
  Config::set("coucou", "coucou");
  BOOST_REQUIRE(Config::get("coucou", "") == "coucou");
  Config::set("new", "hello");
  BOOST_REQUIRE(Config::get("new", "") == "hello");
  Config::set_int("number", 12, true);
  BOOST_REQUIRE(Config::get_int("number", 0) == 12);

  BOOST_REQUIRE(a == 1);
  Config::close(true);
  BOOST_REQUIRE(Config::read_conf("tests/test.conf") == true);
  BOOST_REQUIRE(Config::get_int("number", 0) == 12);
  Config::close(true);
}

BOOST_AUTO_TEST_SUITE_END()
