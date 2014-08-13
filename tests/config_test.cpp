#include <config/config.hpp>
#include <iostream>
#include <fstream>

#include <gtest/gtest.h>

class ConfigTest: public testing::Test
{
protected:

  void SetUp()
  {
    std::ofstream os("tests/test.conf");
    os << "  coucou=machin" << std::endl;
    os << "number=  2" << std::endl;
    os << "empty  =" << std::endl;
    os.close();

    a = 0;
  }

  int a;
};


TEST_F(ConfigTest, basic)
{
  EXPECT_EQ(Config::read_conf("tests/test.conf"), true);
  Config::connect([&]() {++a;});
  EXPECT_EQ(Config::get("coucou", ""), "machin");
  EXPECT_EQ(Config::get("does not exist", "zizi"), "zizi");
  EXPECT_EQ(Config::get_int("number", 0), 2);
  EXPECT_EQ(Config::get_int("does not exist", 1), 1);
  Config::set("coucou", "coucou");
  EXPECT_EQ(Config::get("coucou", ""), "coucou");
  Config::set("new", "hello");
  EXPECT_EQ(Config::get("new", ""), "hello");
  Config::set_int("number", 12, true);
  EXPECT_EQ(Config::get_int("number", 0), 12);

  EXPECT_EQ(a, 1);
  Config::close(true);
  EXPECT_EQ(Config::read_conf("tests/test.conf"), true);
  EXPECT_EQ(Config::get_int("number", 0), 12);
  Config::close(true);
}
