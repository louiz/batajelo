#include <config/config.hpp>

#define CONF_DEFAULT_PATH "./batajelo.conf"

Config* Config::instance = 0;

bool Config::read_conf(const std::string& filename)
{
  if (instance == 0)
    {
      std::ifstream file;
      file.open(filename.data());
      if (!file.is_open())
	{
	  std::cerr << "Error: cannot open config file " << filename << std::endl;
	  return false;
	}
      instance = new Config();
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
	  boost::algorithm::trim_all(value);
	  boost::algorithm::trim_all(option);
	  instance->values[option] = value;
	}
      file.close();
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
  if (instance == 0)
    {
      // We can use a default path, only for testing purpose, to avoid
      // the boring Config::read_conf() at the start of every test suite
      Config::read_conf(CONF_DEFAULT_PATH);
    }

  std::map<std::string, std::string>::iterator it = instance->values.find(option);

  if (it == instance->values.end())
    return def;
  return it->second;
}

void Config::set(const std::string& option, const std::string& value)
{
  if (instance == 0)
    {
      std::cerr << "Error: Config::read_conf() has never been called" << std::endl;
      return ;
    }
  instance->values[option] = value;
  instance->save_to_file();
}

void Config::save_to_file()
{
  std::ofstream file(this->filename.data());
  if (file.fail())
    {
      std::cerr << "Could not save config file." << std::endl;
      return ;
    }
  std::map<std::string, std::string>::iterator it;
  for (it=this->values.begin(); it != this->values.end(); ++it)
    {
      std::cout << it->first <<std::endl;
      file << it->first << "=" << it->second << std::endl;
    }
  file.close();
}
