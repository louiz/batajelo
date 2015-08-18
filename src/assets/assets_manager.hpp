#ifndef ASSETS_MANAGER_HPP_INCLUDED
#define ASSETS_MANAGER_HPP_INCLUDED

#include <boost/filesystem/path.hpp>

#include <string>

class AssetsManager
{
public:
  static void set_asset_directory(const boost::filesystem::path& path);
  static std::string full_name(const boost::filesystem::path& path);

private:
  static boost::filesystem::path data_path;

  AssetsManager() = delete;
  ~AssetsManager() = delete;
  AssetsManager(const AssetsManager&) = delete;
  AssetsManager(AssetsManager&&) = delete;
  AssetsManager& operator=(const AssetsManager&) = delete;
  AssetsManager& operator=(AssetsManager&&) = delete;
};

#endif /* ASSETS_MANAGER_HPP_INCLUDED */
