#include <istream>

#include "server.hpp"
#include "remote_client.hpp"

// main de test
int	main(void)
{
  Config::read_conf("../config/server.conf");
  Server server(7878);
  server.run();
  return 0;
}
