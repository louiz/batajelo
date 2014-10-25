#include "fixmath/fix16.hpp"

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

  Fix16 hypot(Fix16 x, Fix16 y)
  {
    // Avoid overflow by dividing everything by m during the intermediate
    // steps. Then multiply the result by m before returning.  If m is too
    // big, we lose some precision, if it's too small we will overflow
    // anyway.
    // So, first find the ideal value of m.
    static const Fix16 max(181); // sqrt(Fix16Max)
    int m = 2;
    while (x/m >= max || y/m >= max)
      m *= m;

    x /= m;
    y /= m;
    Fix16 t = x*x + y*y;
    return std::sqrt(t)*m;
  }
}
