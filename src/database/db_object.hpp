// Objects of the tables

//string
#include <iostream>
#include <string>
//map
#include <map>
//vector
#include <vector>

#ifndef __DB_OBJECT_HPP__
# define __DB_OBJECT_HPP__

class DbObject
{
public:
  DbObject();
  ~DbObject();
  const std::string& get(const std::string&);
  void set(std::string, std::string);
  void print();
 	template<typename T>
 	void print_vector(std::vector<T*>);

  std::map<std::string, std::string> values;
};

#endif





