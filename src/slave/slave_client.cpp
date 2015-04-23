#include <slave/slave_client.hpp>
#include <slave/slave.hpp>

#include "slave.pb.h"

#include <logging/logging.hpp>

SlaveClient::SlaveClient(Slave* slave):
  slave(slave)
{
}

void SlaveClient::start()
{
  this->connect("127.0.0.1", 7877,
                [this](const boost::system::error_code& error)
                {
                  if (error)
                    this->on_connection_failed(error);
                  else
                    {
                      this->when_connected();
                      this->on_connection_success();
                    }
                });
}

void SlaveClient::install_callbacks()
{
  this->install_callback("INFO", std::bind(&SlaveClient::info_callback, this, std::placeholders::_1));
  this->install_callback("START", std::bind(&SlaveClient::start_callback, this, std::placeholders::_1));
}

void SlaveClient::on_connection_success()
{
  log_debug("Slave connection established");
}

void SlaveClient::on_connection_failed(const boost::system::error_code& error)
{
  log_debug("Slave connection failed");
}

void SlaveClient::on_connection_closed()
{
  log_debug("Connection closed by remote host");
}

void SlaveClient::info_callback(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::slave::InfoRequest>();
  // TODO get info from slave
  ser::slave::SlaveInfo info;
  this->slave->fill_info(info);
}

void SlaveClient::start_callback(Message* message)
{
  this->slave->start_game();
}
