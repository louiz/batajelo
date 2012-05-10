/** @addtogroup Network
 *  @{
 */

/**
 * @class GameClient
 */

#ifndef __GAME_CLIENT_HPP__
# define __GAME_CLIENT_HPP__

#include <network/interface_client.hpp>

using boost::asio::ip::tcp;

class GameClient: public InterfaceClient
{
public:
  GameClient();
  ~GameClient();
  /**
   * Install all the default callbacks.
   */
  virtual void install_callbacks();
  virtual void on_connection_closed();

private:
  void connect_handler(boost::function< void(void) >,
		       boost::function< void(void) >,
		       const boost::system::error_code&);
  boost::asio::io_service io_service;
};

#endif /*__CLIENT_HPP__ */
/**@}*/
