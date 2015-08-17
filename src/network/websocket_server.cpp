#include <network/websocket_server.hpp>

#include <functional>

WebsocketServer::WebsocketServer(const short port)
{
  this->server.set_message_handler([this](wspp::connection_hdl hdl, WsServer::message_ptr msg)
                                   {
                                     this->on_message(hdl, msg);
                                   });
  this->server.set_open_handler([this](wspp::connection_hdl hdl)
                                   {
                                     this->on_open(hdl);
                                   });
  this->server.set_close_handler([this](wspp::connection_hdl hdl)
                                   {
                                     this->on_close(hdl);
                                   });

  this->server.init_asio(&IoService::get());
  this->server.listen(port);
  this->server.start_accept();
}
