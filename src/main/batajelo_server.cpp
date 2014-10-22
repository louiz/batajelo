#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/server.hpp>
#include <game/remote_game_client.hpp>
#include <network/remote_client.hpp>
#include <game/game_server.hpp>
#include <game/time.hpp>
#include <iostream>

int main()
{
  std::ios_base::sync_with_stdio(false);
  if (!Config::read_conf("./batajelo.conf"))
    return 1;
  log_debug("Starting server");

  srand(getpid());
  Mod mod("monsters.yaml");
  GameServer s(7879, mod);
  s.start();

  Time time1 = boost::posix_time::microsec_clock::universal_time();
  Time time2 = time1;

  Duration dt;

  while (true)
    {
      time2 = boost::posix_time::microsec_clock::universal_time();
      dt += time2 - time1;
      time1 = time2;
      long i = get_number_of_updates(dt);
      for (; i > 0; --i)
     {
       s.tick();
     }
      s.poll(100);
    }
  return 0;
}
