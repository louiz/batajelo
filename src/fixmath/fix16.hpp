#ifndef __libfixmath_fix16_hpp__
#define __libfixmath_fix16_hpp__

/**
 * TODO: overload the operator+, operator- etc for the case where the
 * non-Fix16 is the left operand, e.g. 2 + Fix16(12).
 * TODO: add -- and ++ operators
 */

#include "fix16.h"
#include <iostream>
#include <serialization/serializable.hpp>

class Fix16: public Serializable
 {
	public:
		fix16_t value;

		Fix16() { value = 0; }
		Fix16(const Fix16 &inValue)  { value = inValue.value;             }
                Fix16(const int inValue) { assert(inValue <= 32767); value = fix16_from_int(inValue); }
                Fix16(const unsigned int inValue) { assert(inValue <= 32767); value = fix16_from_int(inValue); }
                // Fix16(const unsigned int inValue) { value = fix16_from_int(inValue); }
		// Fix16(const fix16_t inValue) { value = inValue;                   }
		Fix16(const float inValue)   { value = fix16_from_float(inValue);  }
		Fix16(const double inValue)  { value = fix16_from_dbl(inValue);   }
		Fix16(const int16_t inValue) { value = fix16_from_int(inValue);   }

		// operator fix16_t() const { return value;                 }
		operator double()  const { return fix16_to_dbl(value);   }
		operator float()   const { return fix16_to_float(value);  }
		operator int16_t() const { return fix16_to_int(value);   }

int16_t toLong() const { return fix16_to_int(value); }

		Fix16 & operator=(const Fix16 &rhs)  { value = rhs.value;             return *this; }
		// Fix16 & operator=(const fix16_t rhs) { value = rhs;                   return *this; }
		// Fix16 & operator=(const double rhs)  { value = fix16_from_dbl(rhs);   return *this; }
		// Fix16 & operator=(const float rhs)   { value = fix16_from_float(rhs); return *this; }
		// Fix16 & operator=(const int16_t rhs) { value = fix16_from_int(rhs);   return *this; }

		Fix16 & operator+=(const Fix16 &rhs)  { value += rhs.value;             return *this; }
		// Fix16 & operator+=(const fix16_t rhs) { value += rhs;                   return *this; }
		Fix16 & operator+=(const double rhs)  { value += fix16_from_dbl(rhs);   return *this; }
		Fix16 & operator+=(const float rhs)   { value += fix16_from_float(rhs); return *this; }
		Fix16 & operator+=(const int16_t rhs) { value += fix16_from_int(rhs);   return *this; }
		Fix16 & operator+=(const int rhs) { value += fix16_from_int(rhs);   return *this; }

		Fix16 & operator-=(const Fix16 &rhs)  { value -= rhs.value;  return *this; }
		// Fix16 & operator-=(const fix16_t rhs) { value -= rhs; return *this; }
		Fix16 & operator-=(const double rhs)  { value -= fix16_from_dbl(rhs);  return *this; }
		Fix16 & operator-=(const float rhs)   { value -= fix16_from_float(rhs);  return *this; }
		Fix16 & operator-=(const int16_t rhs) { value -= fix16_from_int(rhs);  return *this; }
		Fix16 & operator-=(const int rhs) { value -= fix16_from_int(rhs);  return *this; }

		Fix16 & operator*=(const Fix16 &rhs)  { value = fix16_mul(value, rhs.value);  return *this; }
		// Fix16 & operator*=(const fix16_t rhs) { value = fix16_mul(value, rhs); return *this; }
		Fix16 & operator*=(const double rhs)  { value = fix16_mul(value, fix16_from_dbl(rhs));  return *this; }
		Fix16 & operator*=(const float rhs)   { value = fix16_mul(value, fix16_from_float(rhs));  return *this; }
		Fix16 & operator*=(const int16_t rhs) { value *= rhs;  return *this; }
		Fix16 & operator*=(const int rhs) { value *= rhs;  return *this; }

		Fix16 & operator/=(const Fix16 &rhs)  { value = fix16_div(value, rhs.value);  return *this; }
		// Fix16 & operator/=(const fix16_t rhs) { value = fix16_div(value, rhs); return *this; }
		Fix16 & operator/=(const double rhs)  { value = fix16_div(value, fix16_from_dbl(rhs));  return *this; }
		Fix16 & operator/=(const float rhs)   { value = fix16_div(value, fix16_from_float(rhs));  return *this; }
		Fix16 & operator/=(const int16_t rhs) { value /= rhs;  return *this; }
		Fix16 & operator/=(const int rhs) { value /= rhs; return *this; }

		const Fix16 operator+(const Fix16 &other) const  { Fix16 ret = *this; ret += other; return ret; }
		// const Fix16 operator+(const fix16_t other) const { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const double other) const  { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const float other) const   { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const int16_t other) const { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const int other) const { Fix16 ret = *this; ret += other; return ret; }

		const Fix16 sadd(const Fix16 &other)  const { Fix16 ret = fix16_sadd(value, other.value);             return ret; }
		// const Fix16 sadd(const fix16_t other) const { Fix16 ret = fix16_sadd(value, other);                   return ret; }
		const Fix16 sadd(const double other)  const { Fix16 ret = fix16_sadd(value, fix16_from_dbl(other));   return ret; }
		const Fix16 sadd(const float other)   const { Fix16 ret = fix16_sadd(value, fix16_from_float(other)); return ret; }
		const Fix16 sadd(const int16_t other) const { Fix16 ret = fix16_sadd(value, fix16_from_int(other));   return ret; }
		const Fix16 sadd(const int other) const { Fix16 ret = fix16_sadd(value, fix16_from_int(other));   return ret; }

		const Fix16 operator-(const Fix16 &other) const  { Fix16 ret = *this; ret -= other; return ret; }
		// const Fix16 operator-(const fix16_t other) const { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const double other) const  { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const float other) const   { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const int16_t other) const { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const int other) const { Fix16 ret = *this; ret -= other; return ret; }

   const Fix16 operator-() const { Fix16 ret = *this; return ret * Fix16(-1); }

		const Fix16 ssub(const Fix16 &other)  const { Fix16 ret = fix16_sadd(value, -other.value);             return ret; }
		// const Fix16 ssub(const fix16_t other) const { Fix16 ret = fix16_sadd(value, -other);                   return ret; }
		const Fix16 ssub(const double other)  const { Fix16 ret = fix16_sadd(value, -fix16_from_dbl(other));   return ret; }
		const Fix16 ssub(const float other)   const { Fix16 ret = fix16_sadd(value, -fix16_from_float(other)); return ret; }
		const Fix16 ssub(const int16_t other) const { Fix16 ret = fix16_sadd(value, -fix16_from_int(other));   return ret; }
		const Fix16 ssub(const int other) const { Fix16 ret = fix16_sadd(value, -fix16_from_int(other));   return ret; }

		const Fix16 operator*(const Fix16 &other) const  { Fix16 ret = *this; ret *= other; return ret; }
		// const Fix16 operator*(const fix16_t other) const { Fix16 ret = *this; ret *= other; return ret; }
		const Fix16 operator*(const double other) const  { Fix16 ret = *this; ret *= other; return ret; }
		const Fix16 operator*(const float other) const   { Fix16 ret = *this; ret *= other; return ret; }
		const Fix16 operator*(const int16_t other) const { Fix16 ret = *this; ret *= other; return ret; }
		const Fix16 operator*(const int other) const { Fix16 ret = *this; ret *= other; return ret; }

		const Fix16 smul(const Fix16 &other)  const { Fix16 ret = fix16_smul(value, other.value);             return ret; }
		// const Fix16 smul(const fix16_t other) const { Fix16 ret = fix16_smul(value, other);                   return ret; }
		const Fix16 smul(const double other)  const { Fix16 ret = fix16_smul(value, fix16_from_dbl(other));   return ret; }
		const Fix16 smul(const float other)   const { Fix16 ret = fix16_smul(value, fix16_from_float(other)); return ret; }
		const Fix16 smul(const int16_t other) const { Fix16 ret = fix16_smul(value, fix16_from_int(other));   return ret; }
		const Fix16 smul(const int other) const { Fix16 ret = fix16_smul(value, fix16_from_int(other));   return ret; }

		const Fix16 operator/(const Fix16 &other) const  { Fix16 ret = *this; ret /= other; return ret; }
		// const Fix16 operator/(const fix16_t other) const { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const double other) const  { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const float other) const   { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const int16_t other) const { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const int other) const { Fix16 ret = *this; ret /= other; return ret; }

		const Fix16 sdiv(const Fix16 &other)  const { Fix16 ret = fix16_sdiv(value, other.value);             return ret; }
		// const Fix16 sdiv(const fix16_t other) const { Fix16 ret = fix16_sdiv(value, other);                   return ret; }
		const Fix16 sdiv(const double other)  const { Fix16 ret = fix16_sdiv(value, fix16_from_dbl(other));   return ret; }
		const Fix16 sdiv(const float other)   const { Fix16 ret = fix16_sdiv(value, fix16_from_float(other)); return ret; }
		const Fix16 sdiv(const int16_t other) const { Fix16 ret = fix16_sdiv(value, fix16_from_int(other));   return ret; }
		const Fix16 sdiv(const int other) const { Fix16 ret = fix16_sdiv(value, fix16_from_int(other));   return ret; }

		bool operator==(const Fix16 &other)  const { return (value == other.value);             }
		// bool operator==(const fix16_t other) const { return (value == other);                   }
		bool operator==(const double other)  const { return (value == fix16_from_dbl(other));   }
		bool operator==(const float other)   const { return (value == fix16_from_float(other)); }
		bool operator==(const int16_t other) const { return (value == fix16_from_int(other));   }
		bool operator==(const int other) const { return (value == fix16_from_int(other));   }

		bool operator!=(const Fix16 &other)  const { return (value != other.value);             }
		// bool operator!=(const fix16_t other) const { return (value != other);                   }
		bool operator!=(const double other)  const { return (value != fix16_from_dbl(other));   }
		bool operator!=(const float other)   const { return (value != fix16_from_float(other)); }
		bool operator!=(const int16_t other) const { return (value != fix16_from_int(other));   }
		bool operator!=(const int other) const { return (value != fix16_from_int(other));   }

		bool operator<=(const Fix16 &other)  const { return (value <= other.value);             }
		// bool operator<=(const fix16_t other) const { return (value <= other);                   }
		bool operator<=(const double other)  const { return (value <= fix16_from_dbl(other));   }
		bool operator<=(const float other)   const { return (value <= fix16_from_float(other)); }
		bool operator<=(const int16_t other) const { return (value <= fix16_from_int(other));   }
		bool operator<=(const int other) const { return (value <= fix16_from_int(other));   }

		bool operator>=(const Fix16 &other)  const { return (value >= other.value);             }
		// bool operator>=(const fix16_t other) const { return (value >= other);                   }
		bool operator>=(const double other)  const { return (value >= fix16_from_dbl(other));   }
		bool operator>=(const float other)   const { return (value >= fix16_from_float(other)); }
		bool operator>=(const int16_t other) const { return (value >= fix16_from_int(other));   }
		bool operator>=(const int other) const { return (value >= fix16_from_int(other));   }

		bool operator< (const Fix16 &other)  const { return (value <  other.value);             }
		// bool operator< (const fix16_t other) const { return (value <  other);                   }
		bool operator< (const double other)  const { return (value <  fix16_from_dbl(other));   }
		bool operator< (const float other)   const { return (value <  fix16_from_float(other)); }
		bool operator< (const int16_t other) const { return (value <  fix16_from_int(other));   }
   		bool operator< (const int other) const { return (value <  fix16_from_int(other));   }

		bool operator> (const Fix16 &other)  const { return (value >  other.value);             }
		// bool operator> (const fix16_t other) const { return (value >  other);                   }
		bool operator> (const double other)  const { return (value >  fix16_from_dbl(other));   }
		bool operator> (const float other)   const { return (value >  fix16_from_float(other)); }
		bool operator> (const int16_t other) const { return (value >  fix16_from_int(other));   }
		bool operator> (const int other) const { return (value >  fix16_from_int(other));   }

                Fix16  sin() const { return fix16_to_dbl(fix16_sin(value));  }
		Fix16  cos() const { return fix16_to_dbl(fix16_cos(value));  }
		Fix16  tan() const { return fix16_to_dbl(fix16_tan(value));  }
		// Fix16 asin() const { return fix16_to_dbl(fix16_asin(value)); }
		// Fix16 acos() const { return fix16_to_dbl(fix16_acos(value)); }
		// Fix16 atan() const { return fix16_to_dbl(fix16_atan(value)); }
		// Fix16 atan2(const Fix16 &inY) const { return fix16_to_dbl(fix16_atan2(value, inY.value)); }
		Fix16 sqrt() const { return fix16_to_dbl(fix16_sqrt(value)); }

   virtual void serialize(oarchive & ar, const unsigned int)
   {
     ar & value;
   }
   virtual void serialize(iarchive & ar, const unsigned int)
   {
     ar & value;
   }

};

inline std::ostream& operator<<(std::ostream& os, const Fix16& v)
{
  return os << static_cast<double>(v);
}


#endif
