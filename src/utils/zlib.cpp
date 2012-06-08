#include <utils/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>

std::string zlib_decompress(const std::string& data)
{
  boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
  in.push(boost::iostreams::zlib_decompressor());
  std::stringstream ss(data);
  in.push(ss);
  std::string decompressed;
  boost::iostreams::copy(in, boost::iostreams::back_inserter(decompressed));
  return decompressed;
}

std::string zlib_compress(const std::string& data)
{
  boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
  in.push(boost::iostreams::zlib_compressor());
  std::stringstream ss(data);
  in.push(ss);
  std::string compressed;
  boost::iostreams::copy(in, boost::iostreams::back_inserter(compressed));
  return compressed;
}

