#include "fixmath/fix16.hpp"

// Needed for operator<<(ostream, double)
#include <ostream>

Fix16 operator ""_fix(long double in)
{
  return Fix16(static_cast<double>(in));
}

Fix16 operator ""_fix(unsigned long long int in)
{
  return Fix16(static_cast<unsigned int>(in));
}

Fix16 operator ""_fix(const char* in, std::size_t size)
{
  return Fix16(std::string(in, size).data());
}

bool operator==(const Fix16& lhs, const Fix16& rhs)
{
  return lhs.value == rhs.value;
}

bool operator!=(const Fix16& lhs, const Fix16& rhs)
{
  return lhs.value != rhs.value;
}

bool operator<=(const Fix16& lhs, const Fix16& rhs)
{
  return lhs.value <= rhs.value;
}

bool operator<(const Fix16& lhs, const Fix16& rhs)
{
  return lhs.value < rhs.value;
}

bool operator>(const Fix16& lhs, const Fix16& rhs)
{
  return lhs.value > rhs.value;
}

bool operator>=(const Fix16& lhs, const Fix16& rhs)
{
  return lhs.value >= rhs.value;
}

Fix16 operator-(const Fix16& val)
{
  return val * -1;
}

Fix16 operator/(const Fix16& lhs, const Fix16& rhs)
{
  Fix16 res;
  res.raw() = fix16_div(lhs.value, rhs.value);
  return res;
}

Fix16 operator*(const Fix16& lhs, const Fix16& rhs)
{
  Fix16 res;
  res.raw() = fix16_mul(lhs.value, rhs.value);
  return res;
}

Fix16 operator+(const Fix16& lhs, const Fix16& rhs)
{
  Fix16 res;
  res.raw() = fix16_add(lhs.value, rhs.value);
  return res;
}

Fix16 operator-(const Fix16& lhs, const Fix16& rhs)
{
  Fix16 res;
  res.raw() = fix16_sub(lhs.value, rhs.value);
  return res;
}

Fix16 operator%(const Fix16& lhs, const Fix16& rhs)
{
  Fix16 res;
  res.raw() = fix16_mod(lhs.value, rhs.value);
  return res;
}

namespace std
{
  std::string to_string(Fix16 arg)
  {
    char buf[12];
    fix16_to_str(arg.raw(), buf, 10);
    return buf;
  }

  Fix16 sqrt(Fix16 arg)
  {
    return arg.sqrt();
  }

  Fix16 abs(Fix16 arg)
  {
    if (arg < 0_fix)
      return -arg;
    return arg;
  }

  Fix16 hypot(Fix16 x, Fix16 y)
  {
    static const Fix16 a = 0.96943387;
    static const Fix16 b = 0.39782473;

    x = std::abs(x);
    y = std::abs(y);
    if (x >= y)
      return a*x + b*y;
    else
      return a*y + b*x;
  }

  Fix16 floor(Fix16 val)
  {
    val.raw() = fix16_floor(val.value);
    return val;
  }
}

std::ostream& operator<<(std::ostream& os, const Fix16& rhs)
{
  return os << rhs.to_double();
}

