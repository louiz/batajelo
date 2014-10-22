#include <game/remote_game_client.hpp>
#include <game/game_server.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <world/world.hpp>

RemoteGameClient::RemoteGameClient(boost::asio::io_service& io_service):
  RemoteClientBase(io_service),
  server(nullptr)
{
}

RemoteGameClient::~RemoteGameClient()
{
}

void RemoteGameClient::set_game_server(GameServer* server)
{
  this->server = server;
}

void RemoteGameClient::on_connection_closed()
{
  this->server->remove_client(this);
}

void RemoteGameClient::install_callbacks()
{
}

void RemoteGameClient::ok_callback(Command* command)
{
  // todo replace that Event struct by just an int.
  Event ok_event(command);
  if (ok_event.is_valid() == false)
    {
      log_warning("Invalid data for OK command");
      return ;
    }
  if (this->server->validate_action(ok_event.get_id(), this->get_number()) == true)
    this->server->send_ok(ok_event.get_id(), this->get_number());
}

void RemoteGameClient::turn_callback(Command* command)
{
  std::istringstream is(std::string(command->body, command->body_size));
  unsigned int number;
  is >> number;

  if (this->server->validate_turn(number, this->get_number()) == true)
    this->server->send_turn(number, this->get_number());
}
