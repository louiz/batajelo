#ifndef __VEC2_HPP__
# define __VEC2_HPP__

#include <fixmath/fix16.hpp>

#include <ostream>

class Vec2
{
public:
  Fix16 x;
  Fix16 y;

  Vec2() {}
  Vec2(const Vec2& v):
    x(v.x), y(v.y) {}
  Vec2 (const Fix16& X, const Fix16& Y):
    x(X),
    y(Y)
  { }

  bool operator==(const Vec2& v) const
  {
    return x == v.x && y == v.y;
  }
  bool operator!=(const Vec2& v) const
  {
    return !(*this == v);
  }

  Vec2 operator+(const Vec2& v) const
  {
    return {this->x + v.x, this->y + v.y};
  }
  Vec2 operator-(const Vec2& v) const
  {
    return {this->x - v.x, this->y - v.y};
  }
  Vec2 operator-(void) const
  {
    return Vec2(-x, -y);
  }
  Vec2 operator*(const Fix16 s) const
  {
    return Vec2(x*s, y*s);
  }

  Vec2 operator/(const Fix16 s) const
  {
    return Vec2(x/s, y/s);
  }
  // dot product
  Fix16 dot(const Vec2& v) const
  {
    return (x * v.x) + (y * v.y);
  }
  Vec2& operator=(const Vec2& v)
  {
    this->x = v.x;
    this->y = v.y;
    return *this;
  }
  Vec2& set(const Fix16 x, const Fix16 y)
  {
    this->x = x;
    this->y = y;
    return *this;
  }
  Vec2& operator+=(const Vec2& v)
  {
    return *this = (*this + v);
  }
  Vec2& operator-=(const Vec2& v)
  {
    return *this = (*this - v);
  }
  Vec2& operator*=(const Fix16& s)
  {
    return *this = (*this * s);
  }
  Vec2& operator/=( Fix16 d)
  {
    return *this = (*this / d);
  }
  static Fix16 distance(const Vec2& a, const Vec2& b)
  {
    return (a - b).length();
  }
  void rotate(const Fix16 angle)
  {
    const Fix16 l = this->length();
    const Fix16 s = angle.sin();
    const Fix16 c = angle.cos();
    this->x = (this->x * c) - (this->y * s);
    this->y = (this->x * s) + (this->y * c);
    this->set_length(l);
  }
  Fix16 length() const
  {
    return std::hypot(this->x, this->y);
  }
  // Change the vector's length but keep the same direction.
  void set_length(Fix16 new_length)
  {
    Fix16 mult = this->length() / new_length;
    this->x /= mult;
    this->y /= mult;
  }
  // Returns one of the two perpendicular vectors.
  Vec2 perpendicular1() const
  {
    Vec2 res(this->y, -this->x);
    return res;
  }
  Vec2 perpendicular2() const
  {
    Vec2 res(-this->y, this->x);
    return res;
  }

  // The null (0, 0) vector
  static Vec2 zero;
};

inline Vec2 operator*(Fix16 s, const Vec2& v)
{
  return v*s;
}

inline std::ostream& operator<< (std::ostream& o, const Vec2& v)
{
  return o << "Vec2 (" << std::to_string(v.x) << ", " << std::to_string(v.y) << ")";
}

#endif // __VEC2_HPP__
