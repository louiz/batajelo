/**
 * A very simple base class that must be inherited by all network object
 * that needs a socket.
 */

#ifndef BASE_SOCKET
# define BASE_SOCKET

#include <boost/asio.hpp>
#include <network/ioservice.hpp>

class BaseSocket
{
public:
  explicit BaseSocket():
    socket(IoService::get())
  {
  }
  ~BaseSocket() = default;
  const boost::asio::ip::tcp::socket& get_socket() const
  {
    return this->socket;
  }
  boost::asio::ip::tcp::socket& get_socket()
  {
    return this->socket;
  }

private:
  boost::asio::ip::tcp::socket socket;

  BaseSocket(const BaseSocket&) = delete;
  BaseSocket(BaseSocket&&) = delete;
  BaseSocket& operator=(const BaseSocket&) = delete;
  BaseSocket& operator=(BaseSocket&&) = delete;
};

#endif // BASE_SOCKET
