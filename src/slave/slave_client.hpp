#ifndef SLAVE_CLIENT_HPP_INCLUDED
#define SLAVE_CLIENT_HPP_INCLUDED

/**
 * The client part of the slave. It connects and authenticates to the master
 * servers, and then communicate with it (receives orders to create games,
 * send back results, etc)
 */

#include <network/client_base.hpp>
#include <network/tcp_socket.hpp>

class Slave;

class SlaveClient: public ClientBase<TCPSocket>
{
public:
  SlaveClient(Slave* slave);
  ~SlaveClient() = default;
  void start();

private:
  Slave* slave;

  void info_callback(Message* message);
  void start_callback(Message* message);

  void install_callbacks() override final;

  void on_connection_closed() override final;
  void on_connection_failed(const boost::system::error_code&);
  void on_connection_success();

  SlaveClient(const SlaveClient&) = delete;
  SlaveClient(SlaveClient&&) = delete;
  SlaveClient& operator=(const SlaveClient&) = delete;
  SlaveClient& operator=(SlaveClient&&) = delete;
};


#endif /* SLAVE_CLIENT_HPP_INCLUDED */
