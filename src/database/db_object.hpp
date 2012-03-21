// Objects of the tables

//string
#include <iostream>
#include <string>
//map
#include <map>
//vector
#include <vector>
// date stuf
#include "boost/date_time/posix_time/posix_time.hpp"

#ifndef __DB_OBJECT_HPP__
# define __DB_OBJECT_HPP__

class DbObject
{
public:
  DbObject();
  ~DbObject();
  const std::string get(const std::string&) const;
  const int get_int(const std::string&) const;
  boost::posix_time::ptime get_date(const std::string&) const;
  void set(const std::string&, const std::string&);
  void print();
 	template<typename T>
 	void print_vector(std::vector<T*>);

  std::map<std::string, std::string> values;
};

#endif





