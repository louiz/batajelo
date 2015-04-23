#include <menu_client/menu_client.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>
#include <iostream>

int main()
{
  std::ios_base::sync_with_stdio(false);

  MenuClient::context.set_options(  boost::asio::ssl::context::default_workarounds
                                  | boost::asio::ssl::context::no_sslv2
                                  | boost::asio::ssl::context::no_sslv3);

  MenuClient::context.set_verify_mode(boost::asio::ssl::verify_peer);
  MenuClient::context.load_verify_file("../bata_test.cert");
  MenuClient::context.set_verify_callback(boost::asio::ssl::rfc2818_verification("bata.gg"));

  MenuClient client;

  client.run();

  return 0;
}
