/** @addtogroup Network
 *  @{
 */

/**
 * @class Client
 */

#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

#include <network/client_base.hpp>
#include <network/transfer_receiver.hpp>

using boost::asio::ip::tcp;

class Client: public ClientBase
{
public:
  Client();
  ~Client();
  /**
   * Install all the default callbacks.
   */
  virtual void install_callbacks();
  virtual void on_connection_closed();
  /**
   * Called when a file transfer is finished.
   */
  void on_transfer_ended(const TransferReceiver*);

private:
  // void connect_handler(boost::function< void(void) >,
  //       	       boost::function< void(void) >,
  //       	       const boost::system::error_code&);
  /**
   * Called when the server initiate a file transfer with us.
   * Install the temporary callback to receive each file chunk.
   */
  void transfer_init_callback(Command*);

  std::vector<TransferReceiver*> receivers;
  boost::asio::io_service io_service;
};

#endif /*__CLIENT_HPP__ */
/**@}*/
