#include <network/interface_remote_client.hpp>
#include <database/database.hpp>
#include <database/db_object.hpp>

unsigned long int InterfaceRemoteClient::clients_number = 0;

InterfaceRemoteClient::InterfaceRemoteClient(boost::asio::io_service& io_service):
  number(InterfaceRemoteClient::clients_number++)
{
  this->socket = new tcp::socket(io_service);
}

InterfaceRemoteClient::~InterfaceRemoteClient()
{
  log_info("Deleting remote client " << this->number);
  delete this->socket;
}

tcp::socket& InterfaceRemoteClient::get_socket(void)
{
  return *this->socket;
}

void InterfaceRemoteClient::start()
{
  log_debug("Starting InterfaceRemoteClient " << this->number);
  this->install_callbacks();
  this->install_timed_event(boost::bind(&InterfaceRemoteClient::send_ping, this), 2);
  CommandHandler::install_read_handler();
}

void InterfaceRemoteClient::send_ping()
{
  Command* command = new Command;
  command->set_name("PING");
  this->request_answer(command, boost::bind(&InterfaceRemoteClient::on_pong, this, _1), "PONG");
  this->ping_sent();
}

void InterfaceRemoteClient::on_pong(Command*)
{
  this->pong_received();
  log_debug("Current ping: " << this->get_latency() << "micro seconds.");
  this->install_timed_event(boost::bind(&InterfaceRemoteClient::send_ping, this), 2);
}
