#include <config/config.hpp>
#include <logging/logging.hpp>
#include <game/time.hpp>
#include <game/game_client.hpp>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

int main()
{
  std::ios_base::sync_with_stdio(false);
  srandom(getpid());

  auto screen = std::make_shared<Screen>();
  GameClient client(screen);

  client.run();

  return 0;
}

