#include <utils/string.hpp>
#include <algorithm>
#include <cctype>

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

  void ltrim(std::string& s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  }

  void rtrim(std::string& s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  }

  void trim(std::string& s)
  {
    rtrim(s);
    ltrim(s);
  }

}
