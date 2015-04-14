#ifndef IPC_ENDPOINT_HPP_INCLUDED
#define IPC_ENDPOINT_HPP_INCLUDED

/**
 * Uses message queues (man mq_open)
 */

#include <functional>
#include <vector>
#include <string>

#include <boost/asio/posix/stream_descriptor.hpp>

#include <mqueue.h>

class IPCEndpoint
{
public:
  /**
   * Use the given path to open an existing IPC message queue.
   */
  IPCEndpoint(const std::string& path);
  /**
   * Generate the path and use to create a new IPC message queue.
   */
  IPCEndpoint();
  ~IPCEndpoint();

  std::string get_path() const;
  std::size_t get_max_msgsize() const;
  std::string recv();
  void send(const std::string& msg);
  void watch_read(std::function<void(const std::string&)>&& cb);

private:
  /**
   * Whether or not this object is responsible for the destruction of the
   * message queue.  It's true when we are the one who created it, false
   * otherwise.
   */
  bool must_destroy;
  mqd_t mqd;
  std::string mq_name;
  boost::asio::posix::stream_descriptor stream_descriptor;
  std::vector<char> recv_buffer;
  std::function<void(const std::string&)> read_cb;

  void resize_recv_buffer();
  void watch_read();

  IPCEndpoint(const IPCEndpoint&) = delete;
  IPCEndpoint(IPCEndpoint&&) = delete;
  IPCEndpoint& operator=(const IPCEndpoint&) = delete;
  IPCEndpoint& operator=(IPCEndpoint&&) = delete;
};

#endif /* IPC_ENDPOINT_HPP_INCLUDED */
