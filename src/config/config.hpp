/** @addtogroup Config
 *  @{
 */

/**
 * Read the config file and save all the values in a map.
 * Also, a singleton.
 * Config::read_conf("some_path"); should be called first, then
 * Config::get() should be used to access the values in the conf.
 * @class Config
 */

#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string/trim_all.hpp>

class Config
{
public:
  ~Config(){};
  /**
   * returns a value from the config. If it doesn’t exist, use
   * the second argument as the default.
   * @param option The option we want
   * @param def The default value in case the option does not exist
   */
  static std::string get(const std::string&, const std::string&);
  /**
   * Set a value for the given option. And write all the config
   * in the file from which it was read.
   * @param option The option to set
   * @param value The value to use
   */
  static void set(const std::string&, const std::string&);
  /**
   * Read the configuration file at the given path.
   * @param filename The path to open and read.
   */
  static bool read_conf(const std::string&);
  static Config* instance;

private:
  Config(){};
  Config(const Config &){};
  /**
   * Write all the config values into the configuration file
   */
  void save_to_file();

  std::string filename;
  std::map<std::string, std::string> values;
};

#endif // __CONFIG_HPP__
/**@}*/
