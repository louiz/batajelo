// Objects of the tables

//string
#include <iostream>
#include <string>
//map
#include <map>
//pritf
#include <stdio.h>


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
	virtual std::string get_class_name();

  std::map<std::string, std::string> values;
};

#endif





