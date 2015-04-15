#ifndef CHILDREN_HANDLER_HPP_INCLUDED
#define CHILDREN_HANDLER_HPP_INCLUDED

/**
 * Spawn children processes, and associate an object with each child PID.
 *
 * Use boost asio and a signal handler to manage the lifetime of the children
 * and properly wait() for them.
 *
 * Instantiate a communication channel with nanomsg IPC (man nn_ipc)
 */

#include <slave/child_game.hpp>

#include <boost/asio.hpp>

#include <unistd.h>

#include <memory>
#include <map>

class Slave;

class ChildrenHandler
{
public:
  ChildrenHandler(Slave*);
  ~ChildrenHandler() = default;
  void start_subprocess();
  std::size_t size() const;

private:
  Slave* slave;
  void install_signal_handler();
  void on_child_exit();
  boost::asio::signal_set signal_set;

  std::map<pid_t, std::unique_ptr<ChildGame>> children;

  ChildrenHandler(const ChildrenHandler&) = delete;
  ChildrenHandler(ChildrenHandler&&) = delete;
  ChildrenHandler& operator=(const ChildrenHandler&) = delete;
  ChildrenHandler& operator=(ChildrenHandler&&) = delete;
};

#endif /* CHILDREN_HANDLER_HPP_INCLUDED */
