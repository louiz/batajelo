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
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>

typedef std::function<void()> t_config_changed_callback;

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
   * returns a value from the config. If it doesn’t exist, use
   * the second argument as the default.
   * @param option The option we want
   * @param def The default value in case the option does not exist
   */
  static int get_int(const std::string&, const int&);
  /**
   * Set a value for the given option. And write all the config
   * in the file from which it was read if boolean is set.
   * @param option The option to set
   * @param value The value to use
   * @param save if true, save the config file
   */
  static void set(const std::string&, const std::string&, bool save = false);
  /**
   * Set a value for the given option. And write all the config
   * in the file from which it was read if boolean is set.
   * @param option The option to set
   * @param value The value to use
   * @param save if true, save the config file
   */
  static void set_int(const std::string&, const int&, bool save = false);
  /**
   * Read the configuration file at the given path.
   * @param filename The path to open and read.
   */
  static bool read_conf(const std::string&);
  /**
   * Adds a function to a list. This function will be called whenever a
   * configuration change occurs.
   */
  static void connect(t_config_changed_callback);
  /**
   * The uniq config instance.
   */
  static Config* instance;
  /**
   * Close the config file, saving it to the file is save == true.
   */
  static void close(bool save = false);

private:
  Config(){};
  Config(const Config &){};
  /**
   * Write all the config values into the configuration file
   */
  void save_to_file() const;
  /**
   * Call all the callbacks previously registered using connect().
   * This is used to notify any class that a configuration change occured.
   */
  void trigger_configuration_change();

  std::string filename;
  std::map<std::string, std::string> values;
  std::vector<t_config_changed_callback> callbacks;
};

#endif // __CONFIG_HPP__
/**@}*/
