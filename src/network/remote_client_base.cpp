#include <logging/logging.hpp>
#include <network/remote_client_base.hpp>
#include <network/ioservice.hpp>

// In seconds
static constexpr int ping_interval = 10;

unsigned long int RemoteClientBase::clients_number = 0;

RemoteClientBase::RemoteClientBase():
  id(RemoteClientBase::clients_number++)
{
}

RemoteClientBase::~RemoteClientBase()
{
  log_info("Deleting remote client " << this->id);
}

void RemoteClientBase::start()
{
  log_debug("Starting RemoteClientBase " << this->id);
  this->install_callbacks();
  this->install_timed_event(std::bind(&RemoteClientBase::send_ping, this), ping_interval);
  MessageHandler::install_read_handler();
}

void RemoteClientBase::send_ping()
{
  Message* message = new Message;
  message->set_name("PING");
  this->request_answer(message, std::bind(&RemoteClientBase::on_pong, this, std::placeholders::_1), "PONG");
  this->ping_sent();
}

void RemoteClientBase::on_pong(Message*)
{
  this->pong_received();
  log_debug("Current ping: " << this->get_latency() << "micro seconds.");
  this->install_timed_event(std::bind(&RemoteClientBase::send_ping, this), ping_interval);
}
