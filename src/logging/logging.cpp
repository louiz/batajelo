#include <logging/logging.hpp>
#include <config/config.hpp>

Logger* Logger::instance = nullptr;

Logger::Logger():
  log_level(atoi(Config::get("log_level", "0").data())),
  _stream(Config::get("log_file", "") == "" ? &std::cout : new std::ofstream(Config::get("log_file", "").data(), std::ios_base::app)),
  null_stream(new nullstream)
{
}

Logger* Logger::inst()
{
  if (instance == nullptr)
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
