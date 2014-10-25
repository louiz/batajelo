#include <world/vec2.hpp>

#include <gtest/gtest.h>

TEST(Vec2, BasicConstruction)
{
  Vec2 v(12, 23);
  ASSERT_EQ(v.x, 12);
  ASSERT_EQ(v.y, 23);
}

TEST(Vec2, Comparison)
{
  Vec2 v(-12, 300);
  Vec2 w(-12, 300);
  ASSERT_EQ(v, w);

  Vec2 y(18, 0);
  ASSERT_NE(y, v);
}

TEST(Vec2, Addition)
{
  Vec2 v(0, 0);
  Vec2 c(18, -7823);
  Vec2 res = v + c;

  ASSERT_EQ(res, c);
  res += res;
  ASSERT_EQ(res.x, 36);
  ASSERT_EQ(res.y, -15646);
}

TEST(Vec2, Substraction)
{
  Vec2 v(1, 2);
  Vec2 c(14, 15);
  Vec2 res = v - c;

  ASSERT_EQ(res.x, -13);
  ASSERT_EQ(res.y, -13);
  res -= res;
  // TODO, wat
  // ASSERT_EQ(res, Vec2::zero);
}

TEST(Vec2, Multiplication)
{
  Vec2 v(12, -12.5);
  v *= 3;
  ASSERT_EQ(v.x, 36);
  ASSERT_EQ(v.y, -37.5);

  auto w = 10 * v;
  ASSERT_EQ(w.x, 360);
  ASSERT_EQ(w.y, -375);
}

TEST(Vec2, Division)
{
  Vec2 v(888, -3333);
  ASSERT_EQ(v/2, v*0.5);
}

TEST(Vec2, Distance)
{
  Vec2 v(0, 6);
  Vec2 w(8, 0);
  ASSERT_EQ(Vec2::distance(v, w), 10);

  Vec2 z(1038, -200.8);
  ASSERT_EQ(Vec2::distance(z, z), 0);

  Vec2 zero(0, 0);
  ASSERT_EQ(Vec2::distance(z, zero), z.length());
}

TEST(Vec2, Length)
{
  Vec2 v(8, 6);
  ASSERT_EQ(v.length(), 10);

  v.set_length(20);
  ASSERT_EQ(v.length(), 20);
  ASSERT_EQ(v.x, 16);
  ASSERT_EQ(v.y, 12);
}
