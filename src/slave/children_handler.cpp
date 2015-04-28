#include <slave/children_handler.hpp>
#include <slave/child_game.hpp>
#include <network/ioservice.hpp>
#include <logging/logging.hpp>

#include <thread>

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

ChildrenHandler::ChildrenHandler(Slave* slave):
  slave(slave),
  signal_set(IoService::get(), SIGCHLD)
{
  this->install_signal_handler();
}

void ChildrenHandler::install_signal_handler()
{
  this->signal_set.async_wait([this](const boost::system::error_code& error,
                                     int)
                              {
                                log_debug("sigchld received");
                                if (!error)
                                  this->on_child_exit();
                                else
                                  {
                                    log_error("Failed to wait for SIGCHILD: " << error);
                                  }
                              });
}

void ChildrenHandler::on_child_exit()
{
  log_debug("on_child_exit");
  siginfo_t siginfo;
  siginfo.si_pid = 0;

  int res = ::waitid(P_ALL, 0, &siginfo, WEXITED|WNOHANG);
  if (res != 0)
    perror("waitid");
  else if (siginfo.si_pid == 0)
    log_error("SIGCHLD received but no child process exited.");
  else
    {
      pid_t pid = siginfo.si_pid;
      // TODO do things differently if the process exited unexpectedly, see
      // siginfo.si_code and siginfo.si_status
      auto it = this->children.find(pid);
      if (it == this->children.end())
        log_error("Child process number " << pid << " exited but is not managed by us.");
      else
        {
          it->second->on_end();
          this->children.erase(it);
        }
    }
  this->install_signal_handler();
}

void ChildrenHandler::start_subprocess(const uint64_t game_id)
{
  auto child = std::make_unique<ChildGame>(this->slave);

  pid_t pid = fork();
  if (pid == 0)
    {
      const short port = 7879;
      const char* argv[8] = {
        "./batajelo_game_server",
        "-p", std::to_string(port).data(),
        "-i", child->get_ipc_endpoint_path().data(),
        "-g", std::to_string(game_id).data(),
        nullptr
      };
      execv("./batajelo_game_server", const_cast<char* const*>(argv));
    }
  else
    this->children[pid] = std::move(child);
}

std::size_t ChildrenHandler::size() const
{
  return this->children.size();
}
