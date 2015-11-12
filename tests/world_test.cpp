#include <world/vec2.hpp>
#include <world/world.hpp>

#include "approx.hpp"

TEST_CASE("BasicConstruction", "[vec2]")
{
  Vec2 v(12, 23);
  CHECK(v.x == 12);
  CHECK(v.y == 23);
}

TEST_CASE("Comparison", "[vec2]")
{
  Vec2 v(-12, 300);
  Vec2 w(-12, 300);
  CHECK(v == w);

  Vec2 y(18, 0);
  CHECK(y != v);
}

TEST_CASE("Addition vec2", "[vec2]")
{
  Vec2 v(0, 0);
  Vec2 c(18, -7823);
  Vec2 res = v + c;

  CHECK(res == c);
  res += res;
  CHECK(res.x == 36);
  CHECK(res.y == -15646);
}

TEST_CASE("Substraction vec2", "[vec2]")
{
  Vec2 v(1, 2);
  Vec2 c(14, 15);
  Vec2 res = v - c;

  CHECK(res.x == -13);
  CHECK(res.y == -13);
}

TEST_CASE("Multiplication vec2", "[vec2]")
{
  Vec2 v(12, -12.5);
  v *= 3;
  CHECK(v.x == 36);
  CHECK(v.y == -37.5);

  auto w = 10 * v;
  CHECK(w.x == 360);
  CHECK(w.y == -375);
}

TEST_CASE("Division vec2", "[vec2]")
{
  Vec2 v(888, -3333);
  CHECK(v/2 == v*0.5);
}

TEST_CASE("Distance", "[vec2]")
{
  Vec2 v(0, 6);
  Vec2 w(8, 0);
  CHECK(Vec2::distance(v, w) == fix_approx(10));

  Vec2 z(1038, -200.8);
  CHECK(Vec2::distance(z, z) == 0);

  Vec2 zero(0, 0);
  CHECK(Vec2::distance(z, zero) == z.length());
}

TEST_CASE("Length", "[vec2]")
{
  Vec2 v(8, 6);
  CHECK(v.length() == fix_approx(10));

  v.set_length(20);
  CHECK(v.length() == 20);
  CHECK(v.x == fix_approx(16, 0.3));
  CHECK(v.y == fix_approx(12));
}

TEST_CASE("Angle", "[vec2]")
{
  Vec2 v(-1, 1);
  CHECK(v.angle() == fix_approx(134.9));
}

