#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/server.hpp>
#include <network/remote_game_client.hpp>
#include <network/game_server.hpp>
#include <cstdlib>

int main()
{
  if (!Config::read_conf("./batajelo.conf"))
    return 1;
  log_debug("Starting server");
  srand(getpid());
  // Server<RemoteGameClient> s(7879);
  GameServer s(7879);
  s.run();
  return 0;
}
