#include <slave/slave.hpp>
#include <logging/logging.hpp>

int main()
{
  Slave slave;
  slave.run();
  log_debug("Slave exiting...");
}
