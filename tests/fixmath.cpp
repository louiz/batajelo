#include <fixmath/fix16.hpp>

#include "catch.hpp"

TEST_CASE("basic", "[fixmath]")
{
  Fix16 a(-2);
  CHECK(-a == 2);
  Fix16 b(8787);
  CHECK(-b == -8787);
}

TEST_CASE("ComparisonWithPositiveInt", "[fixmath]")
{
  Fix16 a(8);
  CHECK(a == 8);
  CHECK(a == 8u);
  CHECK(a == 8.f);
  CHECK(a == 8.);

  CHECK(8 == a);
  CHECK(8u == a);
  CHECK(8.f == a);
  CHECK(8. == a);

  Fix16 b(9);
  CHECK(a != b);
  CHECK(a != 9);
  CHECK(a != 9u);
  CHECK(a != 9.);
  CHECK(a != 9.f);

  CHECK(a < b);
  CHECK(a < 9);
  CHECK(a < 9u);
  CHECK(a < 9.);
  CHECK(a < 9.f);

  CHECK(b > a);
  CHECK(9 > a);
  CHECK(9u > a);
  CHECK(9. > a);
  CHECK(9.f > a);

  CHECK(b >= a);
  CHECK(9 >= a);
  CHECK(9u >= a);
  CHECK(9. >= a);
  CHECK(9.f >= a);
}

TEST_CASE("ComparisonWithNegativeInt", "[fixmath]")
{
  Fix16 a(-2839);
  CHECK(a == -2839);
  CHECK(a < -8);
  CHECK(a < 2811);

  Fix16 b(-1);
  CHECK(a < b);
  CHECK(a < 8888);
  CHECK(a == -2839.f);
  CHECK(a >= -2839.f);
  CHECK(a <= -2839.f);
}

TEST_CASE("ComparisonWithDouble", "[fixmath]")
{
  Fix16 a(7878.8);

  CHECK(a > 7878);
  CHECK(a < 7879u);
  CHECK(a == 7878.80);
}

TEST_CASE("ToDouble", "[fixmath]")
{
  auto a = 12.001_fix;
  CHECK(a.to_double() > 12.0);
  CHECK(a.to_double() < 12.1);
}

TEST_CASE("OutputToStream", "[fixmath]")
{
  auto a = 12.001_fix;
  std::ostringstream os;
  os << a;
  CHECK(os.str() == "12.001");
}

TEST_CASE("FromString", "[fixmath]")
{
  Fix16 a("-127.87");
  Fix16 b(-127.87);

  CHECK(a == -127.87);
  CHECK(a == b);
}

TEST_CASE("FixLiteral", "[fixmath]")
{
  CHECK(12_fix == 12);

  Fix16 a(12.000);
  CHECK(a == 12_fix);
  CHECK(a == 12.00_fix);
  CHECK(12.0_fix == 12_fix);
  CHECK(12.0_fix == 12_fix);
}

TEST_CASE("IntegerDivision", "[fixmath]")
{
  Fix16 a(12);
  Fix16 b(2);

  CHECK(a/b == 6);
  CHECK(a/2 == 6);
  CHECK(12/b == 6);
  CHECK(12/b == 6.0);
  CHECK(12.0/b == 6.0_fix);
  CHECK(a/2.f == 6.0_fix);
}

TEST_CASE("FloatDivision", "[fixmath]")
{
  Fix16 a(5);
  Fix16 b(-2);

  CHECK(a/b == -2.5);
  CHECK(a/-2 == -2.5);
  CHECK(5/b == -2.5);
  CHECK(5.f/b == -2.5_fix);
}

TEST_CASE("Substraction", "[fixmath]")
{
  Fix16 a(1);
  CHECK(a-a == 0_fix);
  Fix16 b(3001);
  CHECK(a-b == -3000);
  Fix16 c(8.88);
  CHECK(c-c == 0);
  CHECK(c-8 == 0.88);
  CHECK(c-200 == -191.12);
}

TEST_CASE("Multiplication", "[fixmath]")
{
  Fix16 a(100);
  Fix16 b(80.50);

  CHECK(8*a == 800);
  CHECK(b*a == 8050);
  CHECK(202.0_fix*-80.0_fix == -16160);
}

TEST_CASE("Addition", "[fixmath]")
{
  Fix16 a(88);
  CHECK(200+a == 288);

  Fix16 b(10.99);
  CHECK(b + 0.01_fix == 11);

  CHECK(0_fix + 0_fix == 0.00);
}

TEST_CASE("Mod", "[fixmath]")
{
  Fix16 a(88);
  CHECK(a % 10 == 8);

  Fix16 b(10.99);
  CHECK(b % 10 == 0.99);
}

TEST_CASE("IncrementDecrement", "[fixmath]")
{
  Fix16 a(800.2);
  CHECK(a++ == 800.2);
  CHECK(a == 801.2);
  CHECK(++a == 802.2);
  CHECK(a == 802.2);

  for (auto i = 0u; i < 100; ++i)
    a++;
  CHECK(a == 902.2);

  CHECK(a-- == 902.2);
  CHECK(a == 901.2);
  CHECK(--a == 900.2);
  CHECK(a == 900.2);

  for (auto i = 0u; i < 1000; ++i)
    a--;
  CHECK(a == -99.8_fix);
}

TEST_CASE("SelfOperation", "[fixmath]")
{
  Fix16 a(-18.5);
  a *= 2;
  CHECK(a == -37);

  a /= 2;
  CHECK(a == -18.5);

  a *= 0.5;
  CHECK(a == -9.25);

  a += 2;
  CHECK(a == -7.25);

  a -= 2;
  CHECK(a == -9.25);
}

TEST_CASE("String", "[fixmath]")
{
  CHECK(std::to_string(12.87_fix) == "12.87");
}

TEST_CASE("Hypot", "[fixmath]")
{
  Fix16 x(2.78);
  Fix16 y(3.98);

  Fix16 max;
  max.raw() = fix16_maximum;

  Fix16 res = std::hypot(x, y);
  CHECK(res > 4.96);
  CHECK(res < 4.97);

  x = 5000;
  y = 5000;
  res = std::hypot(x, y);
  CHECK(res > 6820);
  CHECK(res < 7072);
}

TEST_CASE("ToInt", "[fixmath]")
{
  CHECK((2.5_fix).to_int() == 3);
  CHECK((2.49999_fix).to_int() == 2);
  CHECK((-2.49999_fix).to_int() == -2);
  CHECK((-2.5_fix).to_int() == -3);
}

TEST_CASE("Floor", "[FixMath]")
{
  CHECK(4 == std::floor(4.999_fix));
  CHECK(5 == (4.999_fix).to_int());

  CHECK(-5 == std::floor(-4.999_fix));
  CHECK(-5 == (-4.999_fix).to_int());

  CHECK(5444 == std::floor(5444_fix));
}

TEST_CASE("Fuck", "[fixmath]")
{
  short c = (500_fix).to_int() / 100;
  CHECK(c == 5);
}

