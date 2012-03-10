#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string/trim_all.hpp>

// Read the config file and save all the values in
// a map.
// Also, a singleton
// Config::read_conf("some_path"); should be called first, then
// Config::get() should be used to access the values in the conf.
class Config
{
public:
  ~Config(){};
  static std::string get(const std::string&, const std::string&);
  static bool read_conf(const std::string&);
  static Config* instance;

private:
  Config(){};
  Config(const Config &){};

  std::map<std::string, std::string> values;
};


#endif // __CONFIG_HPP__
