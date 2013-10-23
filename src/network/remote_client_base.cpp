#include <logging/logging.hpp>
#include <network/remote_client_base.hpp>
#include <database/database.hpp>
#include <database/db_object.hpp>

unsigned long int RemoteClientBase::clients_number = 0;

RemoteClientBase::RemoteClientBase(boost::asio::io_service& io_service):
  CommandHandler(io_service),
  number(RemoteClientBase::clients_number++),
  io_service(io_service)
{
}

RemoteClientBase::~RemoteClientBase()
{
  log_info("Deleting remote client " << this->number);
}

void RemoteClientBase::start()
{
  log_debug("Starting RemoteClientBase " << this->number);
  this->install_callbacks();
  this->install_timed_event(this->io_service, std::bind(&RemoteClientBase::send_ping, this), 2);
  CommandHandler::install_read_handler();
}

void RemoteClientBase::send_ping()
{
  Command* command = new Command;
  command->set_name("PING");
  this->request_answer(command, std::bind(&RemoteClientBase::on_pong, this, std::placeholders::_1), "PONG");
  this->ping_sent();
}

void RemoteClientBase::on_pong(Command*)
{
  this->pong_received();
  log_debug("Current ping: " << this->get_latency() << "micro seconds.");
  this->install_timed_event(this->io_service, std::bind(&RemoteClientBase::send_ping, this), 2);
}
