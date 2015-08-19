#include <network/repetitive_task.hpp>
#include <network/ioservice.hpp>
#include <logging/logging.hpp>
#include <boost/chrono.hpp>

Repeater::Repeater(const FuncType& f,
                   const DurationType& duration):
  f(f),
  timer(IoService::get()),
  duration(duration)
{
  this->repeat_forever();
}

void Repeater::repeat_forever()
{
  this->timer.expires_at(std::chrono::steady_clock::now() + this->duration);

  auto cb = [this](const boost::system::error_code& ec)
    {
      if (ec)
        log_error("Async wait failed: " << ec);
      else if (f() == true)
        this->repeat_forever();
    };
  this->timer.async_wait(cb);
}
