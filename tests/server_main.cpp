#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/server.hpp>
#include <network/remote_game_client.hpp>
#include <network/remote_client.hpp>
#include <network/game_server.hpp>
#include <cstdlib>
 
int main()
{
if (!Config::read_conf("./batajelo.conf"))
return 1;
log_debug("Starting server");
 
Server<RemoteClient> s(7878);
s.start();
s.run();
}