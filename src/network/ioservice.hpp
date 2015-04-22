#ifndef IOSERVICE_HPP_INCLUDED
#define IOSERVICE_HPP_INCLUDED

#include <boost/asio.hpp>

class IoService
{
public:
  IoService();
  ~IoService();
  static boost::asio::io_service& get()
  {
    return IoService::io_service;
  }

private:
  static boost::asio::io_service io_service;

  IoService(const IoService&) = delete;
  IoService(IoService&&) = delete;
  IoService& operator=(const IoService&) = delete;
  IoService& operator=(IoService&&) = delete;
};

#endif /* IOSERVICE_HPP_INCLUDED */
