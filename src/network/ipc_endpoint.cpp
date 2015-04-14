#include <network/ipc_endpoint.hpp>
#include <network/ioservice.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <stdexcept>

#include <errno.h>

#include <logging/logging.hpp>

using namespace std::string_literals;

IPCEndpoint::IPCEndpoint():
  must_destroy(true),
  mqd(-1),
  mq_name{},
  stream_descriptor(IoService::get())
{
retry:
  {
    auto path = boost::filesystem::unique_path();
    this->mq_name = "/"s + path.string();
  }
  log_debug("Trying to create an ipc endpoint : " << this->mq_name);
  mqd_t mqd = ::mq_open(this->mq_name.data(),
                        O_CREAT|O_RDWR|O_EXCL,
                        S_IRUSR|S_IWUSR,
                        nullptr);
  if (mqd == static_cast<mqd_t>(-1))
    {
      if (errno == EEXIST)
        {
          // We're out of luck, we randomly generated a name that we are
          // already using. Try generating a new one
          goto retry;
        }
      else
        throw std::runtime_error("mq_open failed: "s + strerror(errno));
    }
  this->mqd = mqd;
  this->resize_recv_buffer();
}

IPCEndpoint::IPCEndpoint(const std::string& path):
  must_destroy(false),
  mqd(-1),
  mq_name{path},
  stream_descriptor(IoService::get())
{
  log_debug("Trying to open an existing ipc endpoint : " << this->mq_name);
  this->mqd = ::mq_open(this->mq_name.data(),
                        O_RDWR);
  if (this->mqd == static_cast<mqd_t>(-1))
    throw std::runtime_error("mq_open failed: "s + strerror(errno));
  this->resize_recv_buffer();
}

IPCEndpoint::~IPCEndpoint()
{
  log_debug("~IPCEndpoint");
  if (::mq_close(this->mqd) == -1)
    log_error("Failed to close message queue: " << strerror(errno));
  if (this->must_destroy)
    {
      log_debug("Destroying message queue: " << this->mq_name);
      if (::mq_unlink(this->mq_name.data()) == -1)
        log_error("Failed to unlink message queue " << this->mq_name << ": " << strerror(errno));
    }
}

std::string IPCEndpoint::get_path() const
{
  return this->mq_name;
}

void IPCEndpoint::resize_recv_buffer()
{
  mq_attr attr;
  auto res = ::mq_getattr(this->mqd, &attr);
  if (res == -1)
    throw std::runtime_error("mq_getattr failed: "s + strerror(errno));
  this->recv_buffer.resize(attr.mq_msgsize);
}

std::size_t IPCEndpoint::get_max_msgsize() const
{
  return this->recv_buffer.size();
}

std::string IPCEndpoint::recv()
{
  auto size = ::mq_receive(this->mqd, this->recv_buffer.data(),
                           this->recv_buffer.size(), nullptr);
  if (size == -1)
    throw std::runtime_error("mq_receive failed: "s + strerror(errno));
  return {this->recv_buffer.data(),
      static_cast<std::string::size_type>(size)};
}

void IPCEndpoint::send(const std::string& msg)
{
  auto res = ::mq_send(this->mqd, msg.data(), msg.size(), 0);
  if (res == -1)
    throw std::runtime_error("mq_send failed: "s + strerror(errno));
}

void IPCEndpoint::watch_read(std::function<void(const std::string&)>&& cb)
{
  this->stream_descriptor.assign(this->mqd);
  this->read_cb = std::move(cb);
  this->watch_read();
}

void IPCEndpoint::watch_read()
{
  // Do not actually read anything using boost because we are using
  // null_buffers.  Boost will just call our callback when someone is ready
  // to be read, and we do it manually ourself in the callback.
  auto cb = [this](const boost::system::error_code& error,
                   std::size_t)
    {
      if (error)
        log_debug("Error polling on the IPC file descriptor");
      else
        {
          log_debug("Ready to read");
          auto res = this->recv();
          this->read_cb(res);
        }
      this->watch_read();
    };
  this->stream_descriptor.async_read_some(boost::asio::null_buffers(),
                                          cb);
}
