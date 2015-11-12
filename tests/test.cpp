#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <config/config.hpp>
#include <google/protobuf/stubs/common.h>
#include <logging/logging.hpp>

int main( int argc, char* const argv[] )
{
  // Disable all output
  Config::set("log_level", "5");

  int result = Catch::Session().run(argc, argv);

  google::protobuf::ShutdownProtobufLibrary();
  Logger::instance().reset();
  return result;
}

