#include <config/config.hpp>
#include <logging/logging.hpp>
#include <game/game_client.hpp>
#include <assets/assets_manager.hpp>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

#include <boost/program_options.hpp>

int main(int ac, char** av)
{
  std::ios_base::sync_with_stdio(false);
  srandom(getpid());

  std::string data_directory;

  boost::program_options::variables_map vm;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
    ("help,h",
     "Display this help message")

    ("data-dir,d",
     boost::program_options::value<std::string>(&data_directory)->default_value("../data"),
     "The (full or relative) path to the directory containing the various assets");
  AssetsManager::set_asset_directory("../data/");
  try
    {
      boost::program_options::store(boost::program_options::command_line_parser(ac, av).options(desc).run(),
                                    vm);
      boost::program_options::notify(vm);
      }
  catch (std::exception& e)
    {
      std::cout << e.what() << std::endl;
    }
  if (vm.count("help"))
    {
      std::cout << desc;
      return 0;
    }

  AssetsManager::set_asset_directory(data_directory);
  auto screen = std::make_shared<Screen>();
  GameClient client(screen);

  client.run();

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
