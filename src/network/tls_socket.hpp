#ifndef TLS_SOCKET_HPP_INCLUDED
#define TLS_SOCKET_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <network/ioservice.hpp>

#include <logging/logging.hpp>

class TLSSocket
{
public:
  using ssl_socket_type = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;
  TLSSocket(boost::asio::ssl::context& context):
    ssl_socket(IoService::get(), context)
  {
    log_debug("ssl_socket init with context");
  }
  ~TLSSocket() = default;
  const ssl_socket_type::lowest_layer_type& get_socket() const
  {
    return this->ssl_socket.lowest_layer();
  }
  ssl_socket_type::lowest_layer_type& get_socket()
  {
    return this->ssl_socket.lowest_layer();
  }
  ssl_socket_type& get_stream()
  {
    return this->ssl_socket;
  }

  static boost::asio::ssl::context context;

private:
  ssl_socket_type ssl_socket;

  TLSSocket(const TLSSocket&) = delete;
  TLSSocket(TLSSocket&&) = delete;
  TLSSocket& operator=(const TLSSocket&) = delete;
  TLSSocket& operator=(TLSSocket&&) = delete;
};


#endif /* TLS_SOCKET_HPP_INCLUDED */
