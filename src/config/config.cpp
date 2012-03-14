#include "config.hpp"

Config* Config::instance = 0;

bool Config::read_conf(const std::string& filename)
{
  if (instance == 0)
    {
      instance = new Config();
      std::ifstream file;
      file.open(filename.data());
      if (!file.is_open())
	{
	  std::cerr << "Error: cannot open config file " << filename << std::endl;
	  delete instance;
	  instance = 0;
	  return false;
	}

      std::string line;
      size_t pos;
      std::string option;
      std::string value;
      while (file.good())
	{
	  std::getline(file, line);
	  if (line[0] == '#')
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
      std::cerr << "Error: Config::read_conf() has never been called" << std::endl;
      return 0;
    }

  std::map<std::string, std::string>::iterator it = instance->values.find(option);

  if (it == instance->values.end())
    return def;
  return it->second;
}


// int main(int argc, char *argv[])
// {
//   (void)argc;
//   (void)argv;
//   Config::read_conf("./batajelo.conf");
//   std::string res = Config::get("port", "12");
//   std::cout << res << std::endl;
//   return 0;
// }
