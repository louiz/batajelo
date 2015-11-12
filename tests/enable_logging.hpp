#ifndef BATAJELO_ENABLE_LOGGING_HPP
#define BATAJELO_ENABLE_LOGGING_HPP

#include <config/config.hpp>
#include <logging/logging.hpp>
#include <utils/scopeguard.hpp>

/**
 * Calling this macro in a scope will enable logging,
 * for the duration of the scope.
 * Use it to debug a test, but never commit it.
 */

#define enable_logging() \
  Logger::instance().reset(); \
  Config::set("log_level", "0");\
  utils::ScopeGuard g([](){ Logger::instance().reset();Config::set("log_level", "5"); });

#endif //BATAJELO_ENABLE_LOGGING_HPP
