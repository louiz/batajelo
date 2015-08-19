#include <config/config.hpp>
#include <logging/logging.hpp>
#include <network/server.hpp>
#include <game/remote_game_client.hpp>
#include <game/game_server.hpp>
#include <assets/assets_manager.hpp>
#include <utils/time.hpp>
#include <iostream>

#include <network/repetitive_task.hpp>

#include <boost/program_options.hpp>

int main(int ac, char** av)
{

  short port;
  std::string conf_filename;
  std::string ipc_path;
  uint64_t game_id;
  boost::program_options::variables_map vm;

  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help,h",
     "Display this help message")

    ("port,p", boost::program_options::value<short>(&port)->default_value(7879),
     "The port to bind")

    ("ipc,i", boost::program_options::value<std::string>(&ipc_path),
     "If a path is provided, will connect to that message queue and use it to communicate"
     "with some parent process.")

    ("game_id,g", boost::program_options::value<uint64_t>(&game_id),
     "Unique identifier for the game to be started.")


    ("config,c", boost::program_options::value<std::string>(&conf_filename)->default_value("./batajelo.conf"),
     "The filename of the configuration to use");

  try
    {
      boost::program_options::store(boost::program_options::command_line_parser(ac, av).
                                    options(desc).run(), vm);
      boost::program_options::notify(vm);
    }
  catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
      return 1;
    }
  if (vm.count("help"))
    {
      std::cout << desc;
      return 0;
    }

  std::ios_base::sync_with_stdio(false);
  if (!Config::read_conf("./batajelo.conf"))
    return 1;

  AssetsManager::set_asset_directory("../data");

  log_debug("Starting game server, id: " << game_id << ". Port: " << port << " and config file: " << conf_filename
            << " and ipc path [" << ipc_path << "]");

  srand(getpid());
  GameServer s(port, ipc_path, game_id);
  s.start();

  utils::Time last_update = utils::now();
  std::chrono::steady_clock::duration dt{0};

  Repeater ticks_doer([&last_update, &dt, &s]()
                 {
                   auto now = utils::now();
                   dt += now - last_update;
                   last_update = now;

                   auto ticks = utils::get_number_of_ticks(dt);
                   for (; ticks > 0; --ticks)
                     s.tick();
                   return true;
                 },
                 75ms);

  s.run();

  log_debug("Exiting...");
  return 0;
}
