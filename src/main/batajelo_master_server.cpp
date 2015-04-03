#include <master_server/master_server.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>
#include <utils/time.hpp>
#include <iostream>

int main()
{
  std::ios_base::sync_with_stdio(false);
  if (!Config::read_conf("./batajelo.conf"))
    return 1;
  log_debug("Starting master server");
  log_debug(Config::get("db_login", "non"));

  srand(getpid());
  MasterServer s(7878);
  s.start();

  while (s.is_started())
    s.poll(0);

  log_debug("Exiting...");
  return 0;
}
