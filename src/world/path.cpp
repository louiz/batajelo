#include <world/path.hpp>

Path::Path(const mpreal x, const mpreal y):
  x(x),
  y(y)
{
}

Path::Path(const Path& path)
{
  this->x = path.x;
  this->y = path.y;
}

Path& Path::operator=(const Path& path)
{
  this->x = path.x;
  this->y = path.y;
  return *this;
}

Path::~Path()
{
}
