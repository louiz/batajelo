#include <assets/assets_manager.hpp>
#include <boost/filesystem.hpp>
#include <logging/logging.hpp>

boost::filesystem::path AssetsManager::data_path;

void AssetsManager::set_asset_directory(const boost::filesystem::path& path)
{
  AssetsManager::data_path = boost::filesystem::canonical(path);
}

std::string AssetsManager::full_name(const boost::filesystem::path& path)
{
  auto full_path = AssetsManager::data_path / path;
  return full_path.native();
}
