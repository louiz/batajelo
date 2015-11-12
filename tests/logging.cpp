#include <logging/logging.hpp>
#include <config/config.hpp>

#include "io_tester.hpp"

#include "catch.hpp"

TEST_CASE("Basic logging")
{
  Logger::instance().reset();
  GIVEN("A logger with log_level 0")
    {
      Config::set("log_level", "0");
      WHEN("we log some debug text")
        {
          IoTester<std::ostream> out(std::cout);
          log_debug("debug");
          THEN("debug logs are written")
            CHECK(!out.str().empty());
        }
      WHEN("we log some errors")
        {
          IoTester<std::ostream> out(std::cout);
          log_error("error");
          THEN("error logs are written")
            CHECK(!out.str().empty());
        }
    }
  GIVEN("A logger with log_level 3")
    {
      Config::set("log_level", "3");
      WHEN("we log some debug text")
        {
          IoTester<std::ostream> out(std::cout);
          log_debug("debug");
          THEN("nothing is written")
            CHECK(out.str().empty());
        }
      WHEN("we log some errors")
        {
          IoTester<std::ostream> out(std::cout);
          log_error("error");
          THEN("error logs are still written")
            CHECK(!out.str().empty());
        }
    }
}
