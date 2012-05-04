#include <istream>

#include <network/server.hpp>
#include <network/remote_client.hpp>

// main de test
int	main(void)
{
  Config::read_conf("../config/server.conf");
  Server<RemoteClient> server(7878);
  server.run();
  return 0;
}
