#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/server.hpp>
#include <game/remote_game_client.hpp>
#include <network/remote_client.hpp>
#include <game/game_server.hpp>
#include <utils/time.hpp>
#include <iostream>

int main()
{
  std::ios_base::sync_with_stdio(false);
  if (!Config::read_conf("./batajelo.conf"))
    return 1;
  log_debug("Starting server");

  srand(getpid());
  GameServer s(7879);
  s.start();

  utils::Time last_update = utils::now();
  utils::Duration dt{0};

  while (true)
    {
      auto now = utils::now();
      dt += std::chrono::duration_cast<utils::Duration>(now - last_update);
      last_update = now;

      auto ticks = utils::get_number_of_ticks(dt);
      for (; ticks > 0; --ticks)
        s.tick();
      s.poll(100);
    }
  return 0;
}
