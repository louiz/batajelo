#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include <iostream>
#include <sstream>
#include <string>

#ifndef __SERIALIZABLE_HPP__
# define __SERIALIZABLE_HPP__

typedef boost::archive::text_oarchive oarchive;
typedef boost::archive::text_iarchive iarchive;

class Serializable
{
public:
  Serializable() {}
  ~Serializable() {}

  std::string to_string()
  {
    std::ostringstream oss;
    oarchive archive(oss, boost::archive::no_header);
    archive << *this;
    return oss.str();
  }
  void from_string(const std::string& data)
  {
    std::istringstream iss(data);
    iarchive archive(iss, boost::archive::no_header);
    archive >> *this;
  }

  virtual void serialize(oarchive & ar, const unsigned int) = 0;
  virtual void serialize(iarchive & ar, const unsigned int) = 0;
};

#endif // __SERIALIZABLE_HPP__
