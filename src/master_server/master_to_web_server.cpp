#include <master_server/master_to_web_server.hpp>

#include <iostream>

MasterToWebServer::MasterToWebServer(const short port):
  WebsocketServer(port)
{
}

void MasterToWebServer::on_message(wspp::connection_hdl hdl, WsServer::message_ptr msg)
{
  std::cout << "message " << msg->get_payload() << std::endl;
}

void MasterToWebServer::on_open(wspp::connection_hdl hdl)
{
  std::cout << "open" << std::endl;
}

void MasterToWebServer::on_close(wspp::connection_hdl hdl)
{
  std::cout << "close" << std::endl;
}
