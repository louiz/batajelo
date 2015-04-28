#ifndef SLAVE_HPP_INCLUDED
#define SLAVE_HPP_INCLUDED

#include <slave/slave_client.hpp>
#include <slave/children_handler.hpp>

#include <boost/asio/signal_set.hpp>

#include "slave.pb.h"

class Slave
{
public:
  Slave();
  ~Slave() = default;
  void run();
  void start_game(const uint64_t game_id);
  /**
   * Fill the given structure with the information about this slave.
   */
  void fill_info(ser::slave::SlaveInfo& info);

  static short min_port;
  static std::size_t max_children;

private:
  bool running;
  boost::asio::signal_set signal_set;
  SlaveClient client;
  ChildrenHandler children_handler;

  Slave(const Slave&) = delete;
  Slave(Slave&&) = delete;
  Slave& operator=(const Slave&) = delete;
  Slave& operator=(Slave&&) = delete;
};

#endif /* SLAVE_HPP_INCLUDED */

