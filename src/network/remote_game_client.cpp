#include <network/remote_game_client.hpp>
#include <network/game_server.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <world/server_world.hpp>

RemoteGameClient::RemoteGameClient(boost::asio::io_service& io_service,
                                   Server<RemoteGameClient>* server):
  RemoteClientBase(io_service),
  server(server)
{
}

RemoteGameClient::~RemoteGameClient()
{
}

void RemoteGameClient::on_connection_closed()
{
  this->server->remove_client(this);
}

void RemoteGameClient::install_callbacks()
{
  GameServer* game_server = dynamic_cast<GameServer*>(this->server);
  assert(game_server);
  ServerWorld* world = game_server->get_world();

  this->install_callback("OK", std::bind(&RemoteGameClient::ok_callback, this, std::placeholders::_1));
  this->install_callback("T", std::bind(&RemoteGameClient::turn_callback, this, std::placeholders::_1));
  this->install_callback("MOVE", std::bind(&ServerWorld::move_callback, world, std::placeholders::_1));
  this->install_callback("BUILD", std::bind(&ServerWorld::build_callback, world, std::placeholders::_1));
  this->install_callback("SPAWN", std::bind(&ServerWorld::spawn_callback, world, std::placeholders::_1));
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
  GameServer* game_server = dynamic_cast<GameServer*>(this->server);
  assert(game_server);
  ServerWorld* world = game_server->get_world();

  if (world->validate_action(ok_event.get_id(), this->get_number()) == true)
    game_server->send_ok(ok_event.get_id(), this->get_number());
}

void RemoteGameClient::turn_callback(Command* command)
{
  std::istringstream is(std::string(command->body, command->body_size));
  unsigned int number;
  is >> number;
  GameServer* game_server = dynamic_cast<GameServer*>(this->server);
  assert(game_server);
  ServerWorld* world = game_server->get_world();

  if (world->validate_turn(number, this->get_number()) == true)
    game_server->send_turn(number, this->get_number());
}
