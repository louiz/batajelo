#ifndef libfixmath_fix16_hpp
#define libfixmath_fix16_hpp

#include "fix16.h"

#include <cstddef>
#include <string>
#include <cmath>

class Fix16
 {
 public:
   fix16_t value;

   // Constructors
   Fix16() {}
   Fix16(const int inValue):
     value(fix16_from_int(inValue)) {}
   Fix16(const unsigned int inValue):
     value(fix16_from_int(inValue)) {}
   Fix16(const float inValue):
     value(fix16_from_float(inValue)) {}
   Fix16(const double inValue):
     value(fix16_from_dbl(inValue)) {}
   Fix16(const char* str):
     value(fix16_from_str(str)) {}

   // Everything is compiler-generated, because it's only an int32_t
   // copy/move
   Fix16(Fix16&&) = default;
   Fix16(const Fix16&) = default;
   Fix16& operator=(Fix16&&) = default;
   Fix16& operator=(const Fix16&) = default;

   friend bool operator==(const Fix16& lhs, const Fix16& rhs);
   friend bool operator!=(const Fix16& lhs, const Fix16& rhs);
   friend bool operator<=(const Fix16& lhs, const Fix16& rhs);
   friend bool operator>=(const Fix16& lhs, const Fix16& rhs);
   friend bool operator<(const Fix16& lhs, const Fix16& rhs);
   friend bool operator>(const Fix16& lhs, const Fix16& rhs);

   friend Fix16 operator-(const Fix16& val);

   friend Fix16 operator/(const Fix16& lhs, const Fix16& rhs);
   friend Fix16 operator*(const Fix16& lhs, const Fix16& rhs);
   friend Fix16 operator+(const Fix16& lhs, const Fix16& rhs);
   friend Fix16 operator-(const Fix16& lhs, const Fix16& rhs);
   friend Fix16 operator%(const Fix16& lhs, const Fix16& rhs);

   Fix16& operator++()
   {
     this->value = fix16_add(this->value, fix16_one);
     return *this;
   }

   Fix16 operator++(int)
   {
     Fix16 res = *this;
     this->value = fix16_add(this->value, fix16_one);
     return res;
   }

   Fix16& operator--()
   {
     this->value = fix16_sub(this->value, fix16_one);
     return *this;
   }

   Fix16 operator--(int)
   {
     Fix16 res = *this;
     this->value = fix16_sub(this->value, fix16_one);
     return res;
   }

   Fix16& operator/=(const Fix16& rhs)
   {
     *this = *this / rhs;
     return *this;
   }

   Fix16& operator*=(const Fix16& rhs)
   {
     *this = *this * rhs;
     return *this;
   }

   Fix16& operator-=(const Fix16& rhs)
   {
     *this = *this - rhs;
     return *this;
   }

   Fix16& operator+=(const Fix16& rhs)
   {
     *this = *this + rhs;
     return *this;
   }

   // More mathematical operations
   Fix16 sqrt() const
   {
     Fix16 res;
     res.raw() = fix16_sqrt(this->value);
     return res;
   }
   Fix16 sin() const
   {
     Fix16 res;
     res.raw() = fix16_sin(this->value);
     return res;
   }
   Fix16 cos() const
   {
     Fix16 res;
     res.raw() = fix16_cos(this->value);
     return res;
   }
   Fix16 tan() const
   {
     Fix16 res;
     res.raw() = fix16_tan(this->value);
     return res;
   }
   Fix16 asin() const
   {
     Fix16 res;
     res.raw() = fix16_asin(this->value);
     return res;
   }
   Fix16 acos() const
   {
     Fix16 res;
     res.raw() = fix16_acos(this->value);
     return res;
   }
   Fix16 atan() const
   {
     Fix16 res;
     res.raw() = fix16_atan(this->value);
     return res;
   }
   Fix16 atan2(const Fix16& inY) const
   {
     Fix16 res;
     res.raw() = fix16_atan2(value, inY.value);
     return res;
   }

   // Convert to standard numbers
   int16_t to_int() const
   {
     return fix16_to_int(this->value);
   }
   double to_double() const
   {
     return fix16_to_dbl(this->value);
   }
   fix16_t raw() const
   {
     return this->value;
   }
   fix16_t& raw()
   {
     return this->value;
   }
};

Fix16 operator ""_fix(long double in);
Fix16 operator ""_fix(unsigned long long int in);
Fix16 operator ""_fix(const char* in, std::size_t size);
bool operator==(const Fix16& lhs, const Fix16& rhs);
bool operator!=(const Fix16& lhs, const Fix16& rhs);
bool operator<=(const Fix16& lhs, const Fix16& rhs);
bool operator<(const Fix16& lhs, const Fix16& rhs);
bool operator>(const Fix16& lhs, const Fix16& rhs);
bool operator>=(const Fix16& lhs, const Fix16& rhs);
Fix16 operator-(const Fix16& val);
Fix16 operator/(const Fix16& lhs, const Fix16& rhs);
Fix16 operator*(const Fix16& lhs, const Fix16& rhs);
Fix16 operator+(const Fix16& lhs, const Fix16& rhs);
Fix16 operator-(const Fix16& lhs, const Fix16& rhs);
Fix16 operator%(const Fix16& lhs, const Fix16& rhs);

// Standard overload for the Fix16 type
namespace std
{
  std::string to_string(Fix16 arg);

  Fix16 sqrt(Fix16 arg);

  Fix16 abs(Fix16 arg);

  Fix16 hypot(Fix16 x, Fix16 y);

  Fix16 floor(Fix16 arg);
}

std::ostream& operator<<(std::ostream& os, const Fix16& rhs);

#endif // libfixmath_fix16_hpp
