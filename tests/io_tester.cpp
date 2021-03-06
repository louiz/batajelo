#include "io_tester.hpp"
#include "catch.hpp"
#include <iostream>

/**
 * Directly test this class here
 */
TEST_CASE("Output test", "[io_tester]")
{
  {
    IoTester<std::ostream> out(std::cout);
    std::cout << "test";
    CHECK(out.str() == "test");
  }
  {
    IoTester<std::ostream> out(std::cout);
    CHECK(out.str().empty());
  }
}

TEST_CASE("Input test", "[io_tester]")
{
  {
    IoTester<std::istream> is(std::cin);
    is.set_string("coucou");
    std::string res;
    std::cin >> res;
    CHECK(res == "coucou");
  }
}
