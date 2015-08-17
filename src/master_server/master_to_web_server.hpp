#ifndef MASTER_TO_WEB_SERVER_HPP_INCLUDED
#define MASTER_TO_WEB_SERVER_HPP_INCLUDED

#include <network/websocket_server.hpp>

class MasterToWebServer: public WebsocketServer
{
public:
  MasterToWebServer(const short port);
  ~MasterToWebServer() = default;
  void on_message(wspp::connection_hdl hdl, WsServer::message_ptr msg) override final;
  void on_open(wspp::connection_hdl hdl) override final;
  void on_close(wspp::connection_hdl hdl) override final;

private:
  MasterToWebServer(const MasterToWebServer&) = delete;
  MasterToWebServer(MasterToWebServer&&) = delete;
  MasterToWebServer& operator=(const MasterToWebServer&) = delete;
  MasterToWebServer& operator=(MasterToWebServer&&) = delete;
};


#endif /* MASTER_TO_WEB_SERVER_HPP_INCLUDED */
