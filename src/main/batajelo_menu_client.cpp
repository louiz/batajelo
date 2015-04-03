#include <menu_client/menu_client.hpp>
#include <config/config.hpp>
#include <logging/logging.hpp>
#include <iostream>

int main()
{
  std::ios_base::sync_with_stdio(false);

  MenuClient client;

  client.run();

  return 0;
}
