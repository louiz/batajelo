#include <slave/child_game.hpp>
#include <logging/logging.hpp>

ChildGame::ChildGame(Slave* slave):
  pid(0),
  slave(slave)
{
  this->ipc.watch_read([this](const std::string& msg)
                       {
                         log_debug("Received things from child " << this->pid << ": [" << msg << "]");
                       });
}

void ChildGame::on_end()
{
  log_debug("ChildGame::on_end()");
}

std::string ChildGame::get_ipc_endpoint_path() const
{
  return this->ipc.get_path();
}

void ChildGame::set_pid(const pid_t pid)
{
  this->pid = pid;
}

