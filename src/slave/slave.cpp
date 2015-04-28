#include <slave/slave.hpp>
#include <network/ioservice.hpp>
#include <logging/logging.hpp>

short Slave::min_port = 7790;
std::size_t Slave::max_children = 2;

Slave::Slave():
  running(false),
  signal_set(IoService::get(), SIGINT, SIGTERM),
  client(this),
  children_handler(this)
{
  this->signal_set.async_wait([this](const boost::system::error_code& error,
                                     int)
                              {
                                log_debug("Received SIGINT.");
                                this->running = false;
                              });
}

void Slave::run()
{
  this->running = true;
  this->client.start();
  IoService::get().run();
}

void Slave::start_game(const uint64_t game_id)
{
  this->children_handler.start_subprocess(game_id);
}

void Slave::fill_info(ser::slave::SlaveInfo& info)
{
  info.set_free_slots(Slave::max_children - this->children_handler.size());
}
