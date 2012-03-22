#include <logging/logging.hpp>

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
//   i = Config::get("log_file", "") == "" ? 0 : 1;
//   log_debug("Some debug information: " << i);
//   log_info("some information");
//   log_error("some error");
//   return 0;
// }
