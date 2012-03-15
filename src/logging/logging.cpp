#include "logging.hpp"

Logger* Logger::instance = 0;

Logger::Logger():
  log_level(atoi(Config::get("log_level", "0").data())),
  _stream(Config::get("log_file", "") == "" ? &std::cerr : new std::ofstream(Config::get("log_file", "").data(), std::ios_base::app)),
  null_stream(new nullstream)
{
}

Logger* Logger::inst()
{
  if (instance == 0)
    {
      instance = new Logger;
    }
  return instance;
}

std::ostream& Logger::stream(const int lvl) const
{
  if (lvl >= this->log_level)
    return *(this->_stream);
  return *null_stream;
}

Logger::~Logger()
{
  delete this->_stream;
  delete this->null_stream;
}

// Usage example
// int main(int argc, char *argv[])
// {
//   Config::read_conf("../config/batajelo.conf");
//   int i;
//   Config::get("log_file", "") == "" ? 2 : 1;
//   log_debug << "some debug information " << i << std::endl;
//   log_info << "some information" << std::endl;
//   log_error << "some error" << std::endl;
//   return 0;
// }
