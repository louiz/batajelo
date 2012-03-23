/** @addtogroup Logging
 *  @{
 */

/**
 * Singleton used in logger macros to write into files or stdout, with
 * various levels of severity.
 * Only the macros should be used.
 * @class Logger
 */


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

/**
 * @def log_debug
 * Log a debug message. If the code is compiled as non-debug,
 * the argument is not even computed and nothing happens.
 */
#ifdef DEBUG
# define log_debug(text)
#else
# define log_debug(text)\
  Logger::inst()->stream(debug_lvl) << "[DEBUG]:" << WHERE << ":\t\t" << text << std::endl;
#endif

/**
 * @def log_info
 * Log an info message
 */
#define log_info(text)\
  Logger::inst()->stream(info_lvl) << "[INFO]:" << WHERE << ":\t\t" << text << std::endl;
/**
 * @def log_warning
 * Log a warning message
 */
#define log_warning(text)\
  Logger::inst()->stream(warning_lvl) << "[WARNING]:" << WHERE << ":\t\t" << text << std::endl;
/**
 * @def log_error
 * Log an error message
 */
#define log_error(text)\
  Logger::inst()->stream(error_lvl) << "[ERROR]:" << WHERE << ":\t\t" << text << std::endl;

/**
 * Juste a structure representing a stream doing nothing with its input.
 */
struct nullstream: public std::ostream
{
  nullstream():
    std::ostream(0)
  { }
};

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
/**@}*/
