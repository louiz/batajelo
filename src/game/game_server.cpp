#include <game/game_server.hpp>
#include <game/turn.hpp>
#include <algorithm>

#include "game.pb.h"
#include "orders.pb.h"
#include "requests.pb.h"

GameServer::GameServer(short port):
  Game(),
  Server<RemoteGameClient>(port),
  replay()
{
  this->turn_handler.set_next_turn_callback(std::bind(&GameServer::on_next_turn, this,
                                                      std::placeholders::_1));
  this->start_game();
}

GameServer::~GameServer()
{
}

void GameServer::on_new_client(RemoteGameClient* new_client)
{
  // Give a pointer to ourself (the server) to this new client, so that it
  // can communicate with us later
  new_client->set_game_server(this);

  // First send the new player to all connected clients.
  // (including the just-new-connected one)
  Message* message = new Message();
  message->set_name("NEW_OCCUPANT");

  auto new_occupant = std::make_unique<Occupant>(new_client->get_id(), "coucou");

  // Get the first team not already taken by an other occupant
  uint16_t team;
  for (team = 1;; team++)
    {
      bool res = std::none_of(this->occupants_handler.begin(),
                              this->occupants_handler.end(),
                              [team](const auto& occupant)
                              {
                                return occupant->get_team() == team;
                              });
      if (res)
        break;
    }
  new_occupant->set_team(team);

  message->set_body(new_occupant->serialize());
  this->send_to_all_clients(message);

  this->occupants_handler.add(std::move(new_occupant));
  log_debug("There are now " << this->occupants_handler.size() << " occupants");

  // Then send the list of already-connected clients to the new
  // one.
  for (const auto& occupant: this->occupants_handler)
    {
      if (occupant->id != new_client->get_id())
     { // Do not send the information about itself, it
       // already just received that.
       message = new Message();
       message->set_name("NEW_OCCUPANT");
       message->set_body(occupant->serialize());
       new_client->send(message);
     }
    }
  // Send the replay to the new client.
  this->send_replay(new_client);
  // Send an action to indicate at what turn the replay ends
  // and where the game currently is.
  this->send_start_message(new_client);
  // Send the futur actions to the new client
  this->send_future_actions(new_client);
}

void GameServer::send_replay(RemoteGameClient* new_client)
{
  for (const auto& order: this->replay.get_orders())
    new_client->send(new Message(order));
}

void GameServer::send_future_actions(RemoteGameClient* new_client)
{
  // Turn* turn;
  // Action* action;
  // Message* message;
  // ActionEvent* event;
  // std::size_t occupants_handler = this->occupants_handler.size();

  // for (const auto& turn: this->turn_handler.get_turns())
  //   {
  //     for (const auto& action: turn.get_actions())
  //       {
  //         event = action->get_event();
  //         message = new Message;
  //         message->set_name(event->name);
  //         message->set_body(event->to_string().c_str());
  //         new_client->send(message);
  //       }
  //   }
}

void GameServer::send_start_message(RemoteGameClient* client)
{
  Message* message = new Message;
  ser::order::Start start_msg;
  start_msg.set_turn(this->turn_handler.get_current_turn());
  message->set_name("START");
  message->set_body(start_msg);
  client->send(message);
}

void GameServer::on_client_left(RemoteGameClient* client)
{
  const auto it = std::remove_if(this->occupants_handler.begin(), this->occupants_handler.end(),
                           [client](const auto& occupant)
                                 {
                                   return occupant->id == client->get_id();
                                 });
  std::for_each(it, this->occupants_handler.end(),
               [this](const auto& occupant)
               {
                 Message* message = new Message;
                 message->set_name("OCCUPANT_LEFT");
                 message->set_body(occupant->serialize());
                 this->send_to_all_clients(message);
               });
  this->occupants_handler.erase(it, this->occupants_handler.end());
}

void GameServer::tick()
{
  this->turn_handler.tick();
  if (this->turn_handler.is_paused())
    return;
  this->world.tick();
}

// void GameServer::pause_game()
// {
//   this->started = false;
// }

// void GameServer::unpause_game()
// {
//   this->started = true;
// }

void GameServer::start_game()
{
  this->send_new_entity_order(0, {300, 300}, 1);
  this->send_new_entity_order(0, {400, 800.12}, 1);
  this->send_new_entity_order(0, {500, 500}, 2);
  this->send_new_entity_order(0, {320, 610}, 2);
  this->turn_handler.mark_turn_as_ready();
}

void GameServer::on_next_turn(const TurnNb n)
{
  log_debug("GameServer::on_next_turn(" << n << ")");
  this->turn_handler.mark_turn_as_ready();
  this->send_message_to_all("T", {});
}

void GameServer::send_order_to_all(const char* name, const google::protobuf::Message& srl)
{
  Message* message = new Message;
  message->set_name(name);
  message->set_body(srl);
  this->replay.insert_order(*message);
  this->send_to_all_clients(message);
}

void GameServer::send_message_to_all(const char* name, const std::string& data)
{
  Message* message = new Message;
  message->set_name(name);
  message->set_body(data.data(), data.size());
  this->send_to_all_clients(message);
}

void GameServer::send_turn()
{
  Message* message = new Message;
  message->set_name("T");
  this->send_to_all_clients(message);
}

void GameServer::init()
{
}

void GameServer::on_move_request(Message* message)
{
  auto srl = message->parse_body_to_protobuf_object<ser::request::Move>();
  if (!srl.IsInitialized())
    {
      log_error("Invalid data received for Move request : " << srl.InitializationErrorString());
      return ;
    }
  std::vector<EntityId> ids;
  for (const auto& id: srl.entity_id())
    ids.push_back(id);
  Position pos;
  pos.x.raw() = srl.mutable_pos()->x();
  pos.y.raw() = srl.mutable_pos()->y();
  this->send_move_order(ids, pos, srl.queue());
}

void GameServer::send_new_entity_order(const EntityType type, const Position& pos,
                                       const uint16_t team)
{
  ser::order::NewEntity srl;
  srl.set_turn(this->turn_handler.get_current_turn() + 2);
  srl.set_type_id(type);
  srl.mutable_pos()->set_x(pos.x.raw());
  srl.mutable_pos()->set_y(pos.y.raw());
  srl.set_team(team);
  this->send_order_to_all("NEW_ENTITY", srl);
  this->turn_handler.insert_action(std::bind(&World::do_new_entity, &this->world, type, pos, team),
                                   srl.turn());
}

void GameServer::send_move_order(const std::vector<EntityId> ids, const Position& pos, const bool queue)
{
  ser::order::Move srl;
  srl.set_turn(this->turn_handler.get_current_turn() + 2);
  for (const EntityId id: ids)
    srl.add_entity_id(id);
  srl.set_queue(queue);
  srl.mutable_pos()->set_x(pos.x.raw());
  srl.mutable_pos()->set_y(pos.y.raw());
  this->send_order_to_all("MOVE", srl);
  this->turn_handler.insert_action(std::bind(&World::do_move, &this->world, ids, pos, queue),
                                   srl.turn());
}
