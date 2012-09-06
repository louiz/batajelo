#include <logging/logging.hpp>

#ifndef __VEC2_HPP__
# define __VEC2_HPP__

#include <mpreal/mpreal.h>
#include <serialization/serializable.hpp>

class Vec2: public Serializable
{
public:
  mpreal x;
  mpreal y;

  ~Vec2() {}
  Vec2(const Vec2& v) {x=v.x; y=v.y;}

  // constructors
  Vec2 (): x( 0.0f ), y( 0.0f ) {}
  Vec2 (mpreal X, mpreal Y): x( X ), y( Y ) {}
  Vec2 (const unsigned int X, const unsigned int Y)
  {
    // See the Fix16 limits at
    // http://code.google.com/p/libfixmath/wiki/Fix16Limits
    assert(X < 32767);
    assert(Y < 32767);
    x = (short)X;
    y = (short)Y;
  }

  Vec2 (const int X, const int Y)
  {
    // See the Fix16 limits at
    // http://code.google.com/p/libfixmath/wiki/Fix16Limits
    assert(X < 32767);
    assert(Y < 32767);
    assert(X > -32768);
    assert(Y > -32768);
    x = (short)X;
    y = (short)Y;
  }

  // vector addition
  Vec2 operator+ (const Vec2& v) const {return Vec2 (x+v.x, y+v.y);}

  // vector subtraction
  Vec2 operator- (const Vec2& v) const {return Vec2 (x-v.x, y-v.y);}

  // unary minus
  Vec2 operator- (void) const {return Vec2 (-x, -y);}

  // vector times scalar product (scale length of vector times argument)
  Vec2 operator* (const mpreal s) const {return Vec2 (x * s, y * s);}

  // vector divided by a scalar (divide length of vector by argument)
  Vec2 operator/ (const mpreal s) const {return Vec2 (x / s, y / s);}

  // dot product
  mpreal dot (const Vec2& v) const {return (x * v.x) + (y * v.y);}


    // assignment
  Vec2& operator= (const Vec2& v) {x=v.x; y=v.y; return *this;}

  // set XY coordinates to given two mpreals
  Vec2& set (const mpreal _x, const mpreal _y)
  {x = _x; y = _y; return *this;}

  // +=
  Vec2& operator+= (const Vec2& v) {return *this = (*this + v);}

  // -=
  Vec2& operator-= (const Vec2& v) {return *this = (*this - v);}

  // *=
  Vec2& operator*= (const mpreal& s) {return *this = (*this * s);}

  // /=
  Vec2& operator/=( mpreal d ) { return *this = (*this / d);}

  // equality/inequality
  bool operator== (const Vec2& v) const {return x==v.x && y==v.y;}
  bool operator!= (const Vec2& v) const {return !(*this == v);}

  // @todo Remove - use @c distance from the Vec2Utilitites header instead.
  // XXX experimental (4-1-03 cwr): is this the right approach?  defining
  // XXX "Vec2 distance (vec3, Vec2)" collided with STL's distance template.
  static mpreal distance(const Vec2& a, const Vec2& b)
  {
    return (a - b).length();
  }

  void rotate(const mpreal angle)
  {
    const mpreal l = this->length();
    const mpreal s = angle.sin();
    const mpreal c = angle.cos();
    this->x = (this->x * c) - (this->y * s);
    this->y = (this->x * s) + (this->y * c);
    this->set_length(l);
  }

  mpreal length() const
  {
    // WORKAROUND 1: x^2 often exceeds the Fix16 limit.  We convert the values
    // to ints to do the calculation, but we lose (a lot of?) precision.
    // const int xi = x.toLong();
    // const int yi = y.toLong();
    // const int res = xi*xi + yi*yi;
    // mpreal t = sqrt(res);
    // return t;

    // WORKAROUND 2: We go through two additional steps, to avoid having any
    // big number during the calculation: we divide x and y by 16 before calculating their power of two. And after the sqrt we multiply by 16 again to find the correct result.
    const int m = 32;
    mpreal t = (x/m)*(x/m) + (y/m)*(y/m);
    mpreal a = (x/m)*(x/m);
    mpreal b = (x/m)*(x/m);
    // log_error("length(): x=" << fix16_to_float(a) << " y=" << b);
    t = t.sqrt();
    return t*m;

    // mpreal t = x*x + y*y;
    // return t.sqrt();
  }

  // Change the vector's length but keep the same direction.
  void set_length(mpreal new_length)
  {
    mpreal mult = this->length() / new_length;
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

  static Vec2 zero;

  virtual void serialize(oarchive & ar, const unsigned int)
  {
    ar & x & y;
  }
  virtual void serialize(iarchive & ar, const unsigned int)
  {
    ar & x & y;
  }

};

inline Vec2 operator*(mpreal s, const Vec2& v) {return v*s;}
inline std::ostream& operator<< (std::ostream& o, const Vec2& v)
{
  return o << "Vec2 (" << static_cast<double>(v.x) << ", " << static_cast<double>(v.y) << ")";
}

#endif // __VEC2_HPP__



    // --------------------------- utility member functions used in OpenSteer

    // return component of vector parallel to a unit basis vector
    // (IMPORTANT NOTE: assumes "basis" has unit magnitude (length==1))

  //   inline Vec2 parallelComponent (const Vec2& unitBasis) const
  //   {
  //     const mpreal projection = this->dot (unitBasis);
  //     return unitBasis * projection;
  //   }

  //   // return component of vector perpendicular to a unit basis vector
  //   // (IMPORTANT NOTE: assumes "basis" has unit magnitude (length==1))

  //   inline Vec2 perpendicularComponent (const Vec2& unitBasis) const
  //   {
  //     return (*this) - parallelComponent (unitBasis);
  //   }

  //   // clamps the length of a given vector to maxLength.  If the vector is
  //   // shorter its value is returned unaltered, if the vector is longer
  //   // the value returned has length of maxLength and is paralle to the
  //   // original input.

  //   Vec2 truncateLength (const mpreal maxLength) const
  //   {
  //     const mpreal maxLengthSquared = maxLength * maxLength;
  //     const mpreal vecLengthSquared = this->lengthSquared ();
  //     if (vecLengthSquared <= maxLengthSquared)
  //       return *this;
  //     else
  //       return (*this) * (maxLength / sqrtXXX (vecLengthSquared));
  //   }

  //   // forces a 3d position onto the XZ (aka y=0) plane

  //   Vec2 setYtoZero (void) const {return Vec2 (this->x, 0, this->z);}

  //   // rotate this vector about the global Y (up) axis by the given angle

  //   Vec2 rotateAboutGlobalY (mpreal angle) const 
  //   {
  //     const mpreal s = sinXXX (angle);
  //     const mpreal c = cosXXX (angle);
  //     return Vec2 ((this->x * c) + (this->z * s),
  //                  (this->y),
  //                  (this->z * c) - (this->x * s));
  //   }

  //   // version for caching sin/cos computation
  //   Vec2 rotateAboutGlobalY (mpreal angle, mpreal& sin, mpreal& cos) const 
  //   {
  //     // is both are zero, they have not be initialized yet
  //     if (sin==0 && cos==0)
  //       {
  //         sin = sinXXX (angle);
  //         cos = cosXXX (angle);
  //       }
  //     return Vec2 ((this->x * cos) + (this->z * sin),
  //                  (this->y),
  //                  (this->z * cos) - (this->x * sin));
  //   }

  //   // if this position is outside sphere, push it back in by one diameter

  //   Vec2 sphericalWrapAround (const Vec2& center, mpreal radius)
  //   {
  //     const Vec2 offset = *this - center;
  //     const mpreal r = offset.length();
  //     if (r > radius)
  //       return *this + ((offset/r) * radius * -2);
  //     else
  //       return *this;
  //   }

  //   // names for frequently used vector constants
  //   static const Vec2 zero;
  //   static const Vec2 side;
  //   static const Vec2 up;
  //   static const Vec2 forward;
  // };


  // ----------------------------------------------------------------------------
  // scalar times vector product ("mpreal * Vec2")


  // return cross product a x b
  // inline Vec2 crossProduct(const Vec2& a, const Vec2& b)
  // {
  //   Vec2 result((a.y * b.z) - (a.z * b.y),
  //               (a.z * b.x) - (a.x * b.z),
  //               (a.x * b.y) - (a.y * b.x));
  //   return result;
  // }


  // ----------------------------------------------------------------------------
  // default character stream output method

  // ----------------------------------------------------------------------------
  // Returns a position randomly distributed inside a sphere of unit radius
  // centered at the origin.  Orientation will be random and length will range
  // between 0 and 1


  // Vec2 RandomVectorInUnitRadiusSphere (void);


  // ----------------------------------------------------------------------------
  // Returns a position randomly distributed on a disk of unit radius
  // on the XZ (Y=0) plane, centered at the origin.  Orientation will be
  // random and length will range between 0 and 1


  // Vec2 randomVectorOnUnitRadiusXZDisk (void);


  // ----------------------------------------------------------------------------
  // Returns a position randomly distributed on the surface of a sphere
  // of unit radius centered at the origin.  Orientation will be random
  // and length will be 1


  // inline Vec2 RandomUnitVector (void)
  // {
  //   return RandomVectorInUnitRadiusSphere().normalize();
  // }


  // ----------------------------------------------------------------------------
  // Returns a position randomly distributed on a circle of unit radius
  // on the XZ (Y=0) plane, centered at the origin.  Orientation will be
  // random and length will be 1


  // inline Vec2 RandomUnitVectorOnXZPlane (void)
  // {
  //   return RandomVectorInUnitRadiusSphere().setYtoZero().normalize();
  // }


  // ----------------------------------------------------------------------------
  // used by limitMaxDeviationAngle / limitMinDeviationAngle below


  // Vec2 vecLimitDeviationAngleUtility (const bool insideOrOutside,
  //                                     const Vec2& source,
  //                                     const mpreal cosineOfConeAngle,
  //                                     const Vec2& basis);


  // ----------------------------------------------------------------------------
  // Enforce an upper bound on the angle by which a given arbitrary vector
  // diviates from a given reference direction (specified by a unit basis
  // vector).  The effect is to clip the "source" vector to be inside a cone
  // defined by the basis and an angle.


  // inline Vec2 limitMaxDeviationAngle (const Vec2& source,
  //                                     const mpreal cosineOfConeAngle,
  //                                     const Vec2& basis)
  // {
  //   return vecLimitDeviationAngleUtility (true, // force source INSIDE cone
  //                                         source,
  //                                         cosineOfConeAngle,
  //                                         basis);
  // }


  // ----------------------------------------------------------------------------
  // Enforce a lower bound on the angle by which a given arbitrary vector
  // diviates from a given reference direction (specified by a unit basis
  // vector).  The effect is to clip the "source" vector to be outside a cone
  // defined by the basis and an angle.


  // inline Vec2 limitMinDeviationAngle (const Vec2& source,
  //                                     const mpreal cosineOfConeAngle,
  //                                     const Vec2& basis)
  // {    
  //   return vecLimitDeviationAngleUtility (false, // force source OUTSIDE cone
  //                                         source,
  //                                         cosineOfConeAngle,
  //                                         basis);
  // }


  // ----------------------------------------------------------------------------
  // Returns the distance between a point and a line.  The line is defined in
  // terms of a point on the line ("lineOrigin") and a UNIT vector parallel to
  // the line ("lineUnitTangent")


  // inline mpreal distanceFromLine (const Vec2& point,
  //                                const Vec2& lineOrigin,
  //                                const Vec2& lineUnitTangent)
  // {
  //   const Vec2 offset = point - lineOrigin;
  //   const Vec2 perp = offset.perpendicularComponent (lineUnitTangent);
  //   return perp.length();
  // }


  // ----------------------------------------------------------------------------
  // given a vector, return a vector perpendicular to it (note that this
  // arbitrarily selects one of the infinitude of perpendicular vectors)


  // Vec2 findPerpendicularIn3d (const Vec2& direction);


  // ----------------------------------------------------------------------------
  // candidates for global utility functions
  //
  // dot
  // cross
  // length
  // distance
  // normalized


  // // length
  // mpreal length (void) const {return sqrtXXX (lengthSquared ());}

  // // length squared
  // mpreal lengthSquared (void) const {return this->dot (*this);}

    // normalize: returns normalized version (parallel to this, length = 1)
    // Vec2 normalize (void) const
    // {
    //   // skip divide if length is zero
    //   const mpreal len = length ();
    //   return (len>0) ? (*this)/len : (*this);
    // }

    // // cross product (modify "*this" to be A x B)
    // // [XXX  side effecting -- deprecate this function?  XXX]
    // void cross(const Vec2& a, const Vec2& b)
    // {
    //   *this = Vec2 ((a.y * b.z) - (a.z * b.y),
    //                 (a.z * b.x) - (a.x * b.z),
    //                 (a.x * b.y) - (a.y * b.x));
    // }
