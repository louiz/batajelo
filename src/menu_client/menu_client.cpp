#include <menu_client/menu_client.hpp>

#include <logging/logging.hpp>

#include "master.pb.h"

MenuClient::MenuClient():
  ClientBase(),
  connected(false)
{
}

void MenuClient::run()
{
  log_debug("run");
  this->connect();
  while (true)
    this->poll();
}

void MenuClient::connect()
{
  ClientBase::connect("127.0.0.1", 7878,
                [this]()
                {
                  this->on_connection_success();
                },
                [this](const boost::system::error_code& error)
                {
                  this->on_connection_failed(error);
                });
}

void MenuClient::on_connection_success()
{
  log_debug("connection success. TODO send AUTH");
  this->connected = true;
  ser::master::Authenticate auth_message;
  auth_message.set_login("louiz");
  auth_message.set_password("new_password");
  this->send_message("AUTH", auth_message);
  this->install_callback_once("AUTH_RESPONSE",
                              [this](Message* message)
                              {
                                this->on_auth_response(message);
                              });
}

void MenuClient::on_connection_failed(const boost::system::error_code& error)
{
  log_debug("Connection failed: " << error);
}


void MenuClient::install_callbacks()
{
  log_debug("install_callbacks");
}

void MenuClient::on_connection_closed()
{
  log_debug("connection closed");
  this->connected = false;
}

void MenuClient::on_auth_response(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::master::AuthResponse>();
  if (srl.result() == ser::master::AuthResponse::SUCCESS)
    {
      log_debug("Logged in!");
    }
  else
    {
      log_debug("failure.");
    }
}
