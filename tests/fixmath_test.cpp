#include <fixmath/fix16.hpp>
#include <gtest/gtest.h>

#include <iostream>

TEST(FixMath, Negative)
{
  Fix16 a(-2);
  ASSERT_EQ(-a, 2);
  Fix16 b(8787);
  ASSERT_EQ(-b, -8787);
}

TEST(FixMath, ComparisonWithPositiveInt)
{
  Fix16 a(8);
  ASSERT_EQ(a, 8);
  ASSERT_EQ(a, 8u);
  ASSERT_EQ(a, 8.f);
  ASSERT_EQ(a, 8.);

  ASSERT_EQ(8, a);
  ASSERT_EQ(8u, a);
  ASSERT_EQ(8.f, a);
  ASSERT_EQ(8., a);

  Fix16 b(9);
  ASSERT_NE(a, b);
  ASSERT_NE(a, 9);
  ASSERT_NE(a, 9u);
  ASSERT_NE(a, 9.);
  ASSERT_NE(a, 9.f);

  ASSERT_LT(a, b);
  ASSERT_LT(a, 9);
  ASSERT_LT(a, 9u);
  ASSERT_LT(a, 9.);
  ASSERT_LT(a, 9.f);

  ASSERT_GT(b, a);
  ASSERT_GT(9, a);
  ASSERT_GT(9u, a);
  ASSERT_GT(9., a);
  ASSERT_GT(9.f, a);

  ASSERT_GE(b, a);
  ASSERT_GE(9, a);
  ASSERT_GE(9u, a);
  ASSERT_GE(9., a);
  ASSERT_GE(9.f, a);
}

TEST(FixMath, ComparisonWithNegativeInt)
{
  Fix16 a(-2839);
  ASSERT_EQ(a, -2839);
  ASSERT_LT(a, -8);
  ASSERT_LT(a, 2811);

  Fix16 b(-1);
  ASSERT_LT(a, b);
  ASSERT_LT(a, 8888);
  ASSERT_EQ(a, -2839.f);
  ASSERT_GE(a, -2839.f);
  ASSERT_LE(a, -2839.f);
}

TEST(FixMath, ComparisonWithDouble)
{
  Fix16 a(7878.8);

  ASSERT_GT(a, 7878);
  ASSERT_LT(a, 7879u);
  ASSERT_EQ(a, 7878.80);
}

TEST(FixMath, ToDouble)
{
  auto a = 12.001_fix;
  ASSERT_GT(a.to_double(), 12.0);
  ASSERT_LT(a.to_double(), 12.1);
}

TEST(FixMath, OutputToStream)
{
  auto a = 12.001_fix;
  std::ostringstream os;
  os << a;
  ASSERT_EQ(os.str(), "12.001");
}

TEST(FixMath, FromString)
{
  Fix16 a("-127.87");
  Fix16 b(-127.87);

  ASSERT_EQ(a, -127.87);
  ASSERT_EQ(a, b);
}

TEST(FixMath, FixLiteral)
{
  ASSERT_EQ(12_fix, 12);

  Fix16 a(12.000);
  ASSERT_EQ(a, 12_fix);
  ASSERT_EQ(a, 12.00_fix);
  ASSERT_EQ(12.0_fix, 12_fix);
  ASSERT_EQ("12.0"_fix, 12_fix);
}

TEST(FixMath, IntegerDivision)
{
  Fix16 a(12);
  Fix16 b(2);

  ASSERT_EQ(a/b, 6);
  ASSERT_EQ(a/2, 6);
  ASSERT_EQ(12/b, 6);
  ASSERT_EQ(12/b, 6.0);
  ASSERT_EQ(12.0/b, 6.0_fix);
  ASSERT_EQ(a/2.f, 6.0_fix);
}

TEST(FixMath, FloatDivision)
{
  Fix16 a(5);
  Fix16 b(-2);

  ASSERT_EQ(a/b, -2.5);
  ASSERT_EQ(a/-2, -2.5);
  ASSERT_EQ(5/b, -2.5);
  ASSERT_EQ(5.f/b, -2.5_fix);
}

TEST(FixMath, Substraction)
{
  Fix16 a(1);
  ASSERT_EQ(a-a, 0_fix);
  Fix16 b(3001);
  ASSERT_EQ(a-b, -3000);
  Fix16 c(8.88);
  ASSERT_EQ(c-c, 0);
  ASSERT_EQ(c-8, 0.88);
  ASSERT_EQ(c-200, -191.12);
}

TEST(FixMath, Multiplication)
{
  Fix16 a(100);
  Fix16 b(80.50);

  ASSERT_EQ(8*a, 800);
  ASSERT_EQ(b*a, 8050);
  ASSERT_EQ(202.0_fix*-80.0_fix, -16160);
}

TEST(FixMath, Addition)
{
  Fix16 a(88);
  ASSERT_EQ(200+a, 288);

  Fix16 b(10.99);
  ASSERT_EQ(b + 0.01_fix, 11);

  ASSERT_EQ(0_fix + 0_fix, 0.00);
}

TEST(FixMath, Mod)
{
  Fix16 a(88);
  ASSERT_EQ(a % 10, 8);

  Fix16 b(10.99);
  ASSERT_EQ(b % 10, 0.99);
}

TEST(FixMath, IncrementDecrement)
{
  Fix16 a(800.2);
  ASSERT_EQ(a++, 800.2);
  ASSERT_EQ(a, 801.2);
  ASSERT_EQ(++a, 802.2);
  ASSERT_EQ(a, 802.2);

  for (auto i = 0u; i < 100; ++i)
    a++;
  ASSERT_EQ(a, 902.2);

  ASSERT_EQ(a--, 902.2);
  ASSERT_EQ(a, 901.2);
  ASSERT_EQ(--a, 900.2);
  ASSERT_EQ(a, 900.2);

  for (auto i = 0u; i < 1000; ++i)
    a--;
  ASSERT_EQ(a, -99.8_fix);
}

TEST(FixMath, SelfOperation)
{
  Fix16 a(-18.5);
  a *= 2;
  ASSERT_EQ(a, -37);

  a /= 2;
  ASSERT_EQ(a, -18.5);

  a *= 0.5;
  ASSERT_EQ(a, -9.25);

  a += 2;
  ASSERT_EQ(a, -7.25);

  a -= 2;
  ASSERT_EQ(a, -9.25);
}

TEST(FixMath, String)
{
  ASSERT_EQ(std::to_string(12.87_fix), "12.87");
}

TEST(FixMath, Hypot)
{
  Fix16 x(2.78);
  Fix16 y(3.98);

  Fix16 max;
  max.raw() = fix16_maximum;

  Fix16 res = std::hypot(x, y);
  ASSERT_GT(res, 4.85);
  ASSERT_LT(res, 4.86);

  x = 5000;
  y = 5000;
  res = std::hypot(x, y);
  ASSERT_GT(res, 7071);
  ASSERT_LT(res, 7072);
}

TEST(FixMath, ToInt)
{
  ASSERT_EQ((2.5_fix).to_int(), 3);
  ASSERT_EQ((2.49999_fix).to_int(), 2);
  ASSERT_EQ((-2.49999_fix).to_int(), -2);
  ASSERT_EQ((-2.5_fix).to_int(), -3);
}

TEST(FixMath, Floor)
{
  ASSERT_EQ(4, std::floor(4.999_fix));
  ASSERT_EQ(5, (4.999_fix).to_int());

  ASSERT_EQ(-5, std::floor(-4.999_fix));
  ASSERT_EQ(-5, (-4.999_fix).to_int());

  ASSERT_EQ(5444, std::floor(5444_fix));
}

TEST(FixMath, Fuck)
{
  short c = (500_fix).to_int() / 100;
  ASSERT_EQ(c, 5);
}

