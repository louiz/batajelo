#include <utils/base64.hpp>
#include <utils/string.hpp>
#include <utils/scopeguard.hpp>
#include <utils/zlib.hpp>

#include <gtest/gtest.h>

TEST(Base64, Both)
{
  std::string hello = "Coucou ici";
  EXPECT_EQ(base64_decode(base64_encode(reinterpret_cast<const unsigned char*>(hello.c_str()), hello.size())),
               "Coucou ici");
}

TEST(Base64, encode)
{
  std::string encoded("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
  EXPECT_EQ(base64_decode(encoded), "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.");
}

TEST(Zlib, decompressAndCompress)
{
  std::string original("coucou coucou");
  EXPECT_EQ(zlib_decompress(zlib_compress(original)), "coucou coucou");
}

TEST(Strings, trim)
{
  std::string original("    coucou\n");
  utils::trim(original);
  EXPECT_EQ(original, "coucou");

  original = "    coucou";
  utils::trim(original);
  EXPECT_EQ(original, "coucou");

  original = "coucou  ";
  utils::trim(original);
  EXPECT_EQ(original, "coucou");

  original = "coucou";
  utils::trim(original);
  EXPECT_EQ(original, "coucou");

  original = "\n\ncoucou \r\n ";
  utils::trim(original);
  EXPECT_EQ(original, "coucou");
}

TEST(Scopeguard, everything)
{
  int i = 5;
  {
    ASSERT_EQ(i, 5);
    utils::ScopeGuard guard([&i]() {--i;});
    ASSERT_EQ(i, 5);
  }
  ASSERT_EQ(i, 4);
  {
    ASSERT_EQ(i, 4);
    utils::ScopeGuard guard;
    guard.add_callback([&i]() {--i;});
    guard.add_callback([&i]() {--i;});
    ASSERT_EQ(i, 4);
  }
  ASSERT_EQ(i, 2);
  {
    ASSERT_EQ(i, 2);
    utils::ScopeGuard guard;
    guard.add_callback([&i]() {--i;});
    ASSERT_EQ(i, 2);
    guard.disable();
  }
  ASSERT_EQ(i, 2);
}
