#include <utils/base64.hpp>
#include <utils/string.hpp>
#include <utils/scopeguard.hpp>
#include <utils/zlib.hpp>
#include <utils/time.hpp>

#include "catch.hpp"

TEST_CASE("Both", "[base64]")
{
  std::string hello = "Coucou ici";
  CHECK(base64_decode(base64_encode(reinterpret_cast<const unsigned char*>(hello.c_str()), hello.size())) ==
               "Coucou ici");
}

TEST_CASE("encode", "[base64]")
{
  std::string encoded("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
  CHECK(base64_decode(encoded) == "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.");
}

TEST_CASE("decompressAndCompress", "[zlib]")
{
  std::string original("coucou coucou");
  CHECK(zlib_decompress(zlib_compress(original)) == "coucou coucou");
}

TEST_CASE("trim", "[strings]")
{
  std::string original("    coucou\n");
  utils::trim(original);
  CHECK(original == "coucou");

  original = "    coucou";
  utils::trim(original);
  CHECK(original == "coucou");

  original = "coucou  ";
  utils::trim(original);
  CHECK(original == "coucou");

  original = "coucou";
  utils::trim(original);
  CHECK(original == "coucou");

  original = "\n\ncoucou \r\n ";
  utils::trim(original);
  CHECK(original == "coucou");
}

TEST_CASE("Scopeguard")
{
  int i = 5;
  {
    CHECK(i == 5);
    utils::ScopeGuard guard([&i]() {--i;});
    CHECK(i == 5);
  }
  CHECK(i == 4);
  {
    CHECK(i == 4);
    utils::ScopeGuard guard;
    guard.add_callback([&i]() {--i;});
    guard.add_callback([&i]() {--i;});
    CHECK(i == 4);
  }
  CHECK(i == 2);
  {
    CHECK(i == 2);
    utils::ScopeGuard guard;
    guard.add_callback([&i]() {--i;});
    CHECK(i == 2);
    guard.disable();
  }
  CHECK(i == 2);
}

TEST_CASE("BasicTick", "[time]")
{
  using namespace std::chrono_literals;

  utils::Duration dt = 1s;
  auto ticks = utils::get_number_of_ticks(dt);
  CHECK(ticks == 50);
  CHECK(dt == 0s);
}

TEST_CASE("BasicTick2", "[time]")
{
  using namespace std::chrono_literals;

  utils::Duration dt = 500000us;
  auto ticks = utils::get_number_of_ticks(dt);
  CHECK(ticks == 25);
  CHECK(dt == 0s);
}

TEST_CASE("Ticks", "[time]")
{
  using namespace std::chrono_literals;

  utils::Duration dt = 420000us;
  auto ticks = utils::get_number_of_ticks(dt);
  CHECK(ticks == 21);
  CHECK(dt == 0us);
}

TEST_CASE("ConvertToFloatingSeconds", "[time]")
{
  using namespace std::chrono_literals;

  utils::Duration dt = 8420000us;
  auto secs = utils::sec(dt);
  CHECK(secs == 8.42s);

  dt = -8420000us;
  secs = utils::sec(dt);
  CHECK(secs == -8.42s);
}

TEST_CASE("NullTime", "[time]")
{
  using namespace std::chrono_literals;

  utils::Duration dt = 0h;
  auto ticks = utils::get_number_of_ticks(dt);
  CHECK(ticks == 0);
  CHECK(dt == 0us);
}
