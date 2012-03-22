#include <config/config.hpp>

#include <iostream>

#ifndef __LOGGING_HPP__
# define __LOGGING_HPP__

#define debug_lvl 0
#define info_lvl 1
#define warning_lvl 2
#define error_lvl 3

#define WHERE\
  __FILE__ << ":" << __LINE__

#ifdef DEBUG
# define log_debug(text)
#else
# define log_debug(text)\
  Logger::inst()->stream(debug_lvl) << "[DEBUG]:" << WHERE << ":\t\t" << text << std::endl;
#endif

#define log_info(text)\
  Logger::inst()->stream(info_lvl) << "[INFO]:" << WHERE << ":\t\t" << text << std::endl;
#define log_warning(text)\
  Logger::inst()->stream(warning_lvl) << "[WARNING]:" << WHERE << ":\t\t" << text << std::endl;
#define log_error(text)\
  Logger::inst()->stream(error_lvl) << "[ERROR]:" << WHERE << ":\t\t" << text << std::endl;

struct nullstream: public std::ostream
{
  nullstream():
    std::ostream(0)
  { }
};

// Singleton used in logger macros to write into files or stdout, with
// various levels of severity.
class Logger
{
public:
  static Logger* inst();
  std::ostream& stream(const int) const;
  ~Logger();

private:
  Logger();
  Logger(const Logger&);
  Logger& operator=(const Logger&);

  int log_level;
  std::ostream* _stream;
  static Logger* instance;
  nullstream* null_stream;
};

#endif // __LOGGING_HPP__
