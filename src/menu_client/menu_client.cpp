#include <menu_client/menu_client.hpp>

#include <logging/logging.hpp>

#include "master.pb.h"

boost::asio::ssl::context MenuClient::context(boost::asio::ssl::context::tlsv1);

MenuClient::MenuClient():
  ClientBase<TLSSocket>(MenuClient::context),
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
         [this](const boost::system::error_code& error)
             {
               if (error)
                 this->on_connection_failed(error);
               else
                 {
                   log_debug("Connected. Doing the handshake now");
                   this->get_stream().async_handshake(TLSSocket::ssl_socket_type::client,
                           [this](const boost::system::error_code& error)
                               {
                                 if (error)
                                   {
                                     log_error("Handshake failed: " << error);
                                     this->get_stream().shutdown();
                                   }
                                 else
                                   {
                                     log_debug("Handshake successful");
                                     this->when_connected();
                                     this->on_connection_success();
                                   }
                               });
                 }
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
