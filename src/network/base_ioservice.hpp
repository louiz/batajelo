/**
 * A very simple base class that must be inherited by all network object
 * that needs their own ioservice (for example anything that has a socket,
 * or a timer, etc). But not by class that need to share a io_service that
 * was created earlier.  Basically, this means only Server, Client and
 * GameClient should inherite this. All remote_client etc need to share the
 * Server's io_service.
 */

#ifndef BASE_IOSERVICE
# define BASE_IOSERVICE

#include <boost/asio.hpp>

class BaseIoservice
{
public:
  explicit BaseIoservice() {}
  ~BaseIoservice() {}
  /**
   * Returns a reference to the io_service object, so other affiliated
   * objects can use it to create some io-related objects.
   */
  boost::asio::io_service& get_io_service()
  {
    return this->io_service;
  }
protected:
  boost::asio::io_service io_service;
private:
  BaseIoservice(const BaseIoservice&) = delete;
  BaseIoservice(BaseIoservice&&) = delete;
  BaseIoservice& operator=(const BaseIoservice&) = delete;
  BaseIoservice& operator=(BaseIoservice&&) = delete;
};

#endif // BASE_IOSERVICE

