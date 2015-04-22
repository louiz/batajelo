#include <sstream>

#include <config/config.hpp>
#include <utils/string.hpp>
#include <utils/scopeguard.hpp>

#include <iostream>

#define CONF_DEFAULT_PATH "./batajelo.conf"

std::unique_ptr<Config> Config::instance;

bool Config::read_conf(const std::string& filename)
{
  if (!instance)
    {
      std::ifstream file;
      file.open(filename.data());
      if (!file.is_open())
        {
          std::cerr << "Error: cannot open config file " << filename << std::endl;
          return false;
        }
      utils::ScopeGuard guard([&file]() {file.close();});
      instance = std::make_unique<Config>();
      instance->filename = filename;

      std::string line;
      size_t pos;
      std::string option;
      std::string value;
      while (file.good())
        {
          std::getline(file, line);
          if (line == "" || line[0] == '#')
            continue ;
          pos = line.find('=');
          if (pos == std::string::npos)
            continue ;
          option = line.substr(0, pos);
          value = line.substr(pos+1);
          utils::trim(value);
          utils::trim(option);
          instance->values[option] = value;
        }
      return true;
    }
  else
    {
      std::cerr << "Error: Config::read_conf() called twice." << std::endl;
      return false;
    }
}

std::string Config::get(const std::string& option, const std::string& def)
{
  if (!instance)
    {
      // We can use a default path, only for testing purpose, to avoid
      // the boring Config::read_conf() at the start of every test suite
      Config::read_conf(CONF_DEFAULT_PATH);
    }

  auto it = instance->values.find(option);

  if (it == instance->values.end())
    return def;
  return it->second;
}

int Config::get_int(const std::string& option, const int& def)
{
  std::string res = get(option, "");
  if (res.size() > 0)
    return atoi(res.c_str());
  else
    return def;
}

void Config::set_int(const std::string& option, const int& value, bool save)
{
  std::ostringstream os;
  os << value;
  Config::set(option, os.str(), save);
}

void Config::set(const std::string& option, const std::string& value, bool save)
{
  if (!instance)
    {
      std::cerr << "Error: Config::read_conf() has never been called" << std::endl;
      return ;
    }
  instance->values[option] = value;
  if (save)
    {
      instance->save_to_file();
      instance->trigger_configuration_change();
    }
}

void Config::save_to_file() const
{
  std::ofstream file(this->filename.data());
  if (file.fail())
    {
      std::cerr << "Could not save config file." << std::endl;
      return ;
    }
  for (auto& it: this->values)
    file << it.first << "=" << it.second << std::endl;
  file.close();
}

void Config::connect(t_config_changed_callback callback)
{
  if (!instance)
    {
      std::cerr << "Error: Config::read_conf() has never been called" << std::endl;
      return ;
    }
  instance->callbacks.push_back(callback);
}

void Config::close(bool save)
{
  if (!instance)
    std::cerr << "Error: Config::close() called but it was not open." << std::endl;
  if (save)
    instance->save_to_file();
  instance->values.clear();
  instance.reset(nullptr);
}

void Config::trigger_configuration_change()
{
  std::vector<t_config_changed_callback>::iterator it;
  for (it = this->callbacks.begin(); it < this->callbacks.end(); ++it)
    {
      (*it)();
    }
}
