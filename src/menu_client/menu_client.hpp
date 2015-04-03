#ifndef MENU_CLIENT_HPP_INCLUDED
#define MENU_CLIENT_HPP_INCLUDED

#include <network/client_base.hpp>

class MenuClient: public ClientBase
{
public:
  MenuClient();
  ~MenuClient() = default;
  void run();
  void connect();
  void on_connection_success();
  void on_connection_failed(const boost::system::error_code& error);
  void install_callbacks() override final;
  void on_connection_closed() override final;
  void on_auth_response(Message* message);

private:
  bool connected;
  MenuClient(const MenuClient&) = delete;
  MenuClient(MenuClient&&) = delete;
  MenuClient& operator=(const MenuClient&) = delete;
  MenuClient& operator=(MenuClient&&) = delete;
};


#endif /* MENU_CLIENT_HPP_INCLUDED */
