#include <utils/string.hpp>

namespace utils
{

std::string basename(const std::string& filename)
{
  std::size_t pos = filename.rfind('.');
  if (pos == std::string::npos)
    {
      return filename;
    }
  return filename.substr(0, pos);
}

}
