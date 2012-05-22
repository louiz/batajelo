#include <logging/logging.hpp>
#include <serialization/serializable.hpp>

#define BOOST_TEST_MODULE serialization
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(serialization_suite1)

class MyBase: public Serializable
{
  friend class boost::serialization::access;
public:
  int x;
  int y;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & this->y & this->x;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & this->y & this->x;
  }
};

class MyDerive: public MyBase
{
public:
  int a;

  virtual void serialize(iarchive & ar, const unsigned int a)
  {
    // The doc says not to do that, but for reasons that do not matter, and
    // that's the only way I've found, so...
    MyBase::serialize(ar, a);
    ar & this->a;
  }
  virtual void serialize(oarchive & ar, const unsigned int)
  {
    MyBase::serialize(ar, a);
    ar & this->a;
  }
};

BOOST_AUTO_TEST_CASE(serialization_test1)
{
  MyBase a;
  MyBase b;
  a.y = 11;
  a.x = 22;
  BOOST_REQUIRE(b.from_string(a.to_string()) == true);
  BOOST_REQUIRE(a.y == b.y);
  BOOST_REQUIRE(a.x == b.x);
}

BOOST_AUTO_TEST_CASE(serialization_test2)
{
  MyBase a;
  MyDerive c;
  c.x = 33;
  c.y = 44;
  c.a = 55;
  BOOST_REQUIRE(a.from_string(c.to_string()) == true);
  BOOST_REQUIRE(a.y == c.y);
  BOOST_REQUIRE(a.x == c.x);
}

BOOST_AUTO_TEST_CASE(serialization_test3)
{
  MyDerive a;
  MyDerive c;
  c.x = 33;
  c.y = 44;
  c.a = 55;
  a.from_string(c.to_string());
  BOOST_REQUIRE(a.y == c.y);
  BOOST_REQUIRE(a.x == c.x);
  BOOST_REQUIRE(a.a == c.a);
}

BOOST_AUTO_TEST_CASE(serialization_test4)
{
  MyBase a;
  BOOST_REQUIRE(false == a.from_string("un anus"));
}

BOOST_AUTO_TEST_SUITE_END()
