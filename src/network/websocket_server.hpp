#ifndef WEBSOCKET_SERVER_HPP_INCLUDED
#define WEBSOCKET_SERVER_HPP_INCLUDED

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <network/ioservice.hpp>

namespace wspp = websocketpp;

using WsServer = wspp::server<websocketpp::config::asio>;

class WebsocketServer
{
public:
  WebsocketServer(const short port);
  ~WebsocketServer() = default;

protected:
  virtual void on_message(wspp::connection_hdl hdl, WsServer::message_ptr msg) = 0;
  virtual void on_open(wspp::connection_hdl hdl) = 0;
  virtual void on_close(wspp::connection_hdl hdl) = 0;

private:
  WsServer server;

  WebsocketServer(const WebsocketServer&) = delete;
  WebsocketServer(WebsocketServer&&) = delete;
  WebsocketServer& operator=(const WebsocketServer&) = delete;
  WebsocketServer& operator=(WebsocketServer&&) = delete;
};


#endif /* WEBSOCKET_SERVER_HPP_INCLUDED */
