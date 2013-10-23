/** @addtogroup Network
 *  @{
 */

/**
 * @class GameClient
 */

#include <vector>

#ifndef __GAME_CLIENT_HPP__
# define __GAME_CLIENT_HPP__

#include <network/client_base.hpp>

using boost::asio::ip::tcp;

class GameClient: public ClientBase
{
public:
  GameClient();
  ~GameClient();
  /**
   * Install all the default callbacks.
   */
  virtual void install_callbacks();
  virtual void on_connection_closed();

  void new_client_callback(Command*);

private:
  void connect_handler(std::function< void(void) >,
        	       std::function< void(void) >,
        	       const boost::system::error_code&);
};

#endif /*__CLIENT_HPP__ */
/**@}*/
