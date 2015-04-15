#ifndef CHILD_GAME_HPP_INCLUDED
#define CHILD_GAME_HPP_INCLUDED

#include <network/ipc_endpoint.hpp>

#include <string>

class Slave;

class ChildGame
{
public:
  ChildGame(Slave* slave);
  ~ChildGame() = default;
  void on_end();
  std::string get_ipc_endpoint_path() const;
  void set_pid(const pid_t pid);

private:
  Slave* slave;
  IPCEndpoint ipc;
  pid_t pid;

  ChildGame(const ChildGame&) = delete;
  ChildGame(ChildGame&&) = delete;
  ChildGame& operator=(const ChildGame&) = delete;
  ChildGame& operator=(ChildGame&&) = delete;
};


#endif /* CHILD_GAME_HPP_INCLUDED */
