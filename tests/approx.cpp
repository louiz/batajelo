#include <approx.hpp>

/**
 * The epsilon is quite big, because Fix16 computations are not precise.
 */
Approx fix_approx(const Fix16& value)
{
  return fix_approx(value, 0.014);
}

Approx fix_approx(const Fix16& value, const double epsilon)
{
  Approx res(value.to_double());
  res.epsilon(epsilon);
  return res;
}

bool operator==(const Fix16& lhs, const Approx& rhs)
{
  return lhs.to_double() == rhs;
}

TEST_CASE("Fix16 Approx", "[approx]")
{
  Fix16 value(2.01);
  CHECK(2.f == fix_approx(value));
  CHECK(18.f != fix_approx(value));

  CHECK(18.f == fix_approx(value, 20.0));

  CHECK(2_fix == fix_approx(value));
}
