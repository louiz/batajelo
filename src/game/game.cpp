#include <game/game.hpp>
#include <orders.pb.h>

Game::Game():
  world(),
  occupants_handler(),
  turn_handler()
{
}

Game::~Game()
{
}


void Game::new_unit_callback(Message* message)
{
  log_debug("Game::new_unit_callback");
  auto srl = message->parse_body_to_protobuf_object<ser::order::NewUnit>();
  if (!srl.IsInitialized())
    {
      log_error("Invalid data received for new unit: " << srl.InitializationErrorString());
      return ;
    }

  Position pos;
  pos.x.raw() = srl.pos().x();
  pos.y.raw() = srl.pos().y();
  this->turn_handler.insert_action(std::bind(&Game::do_new_unit, this, srl.type_id(), pos),
                                   srl.turn());
}

void Game::move_callback(Message* message)
{
  log_debug("Game::move_callback");
  auto srl = message->parse_body_to_protobuf_object<ser::order::Move>();
  if (!srl.IsInitialized())
    {
      log_error("Invalid data received for move: " << srl.InitializationErrorString());
      return ;
    }

  std::vector<EntityId> ids;
  for (const auto& id: srl.entity_id())
    ids.push_back(id);

  Position pos;
  pos.x.raw() = srl.mutable_pos()->x();
  pos.y.raw() = srl.mutable_pos()->y();

  this->turn_handler.insert_action(std::bind(&World::do_move, &this->world, ids, pos, srl.queue()),
                                   srl.turn());
}

void Game::do_new_unit(const EntityType type, const Position& pos)
{
  log_debug("Game::do_new_unit");
  this->world.do_new_unit(type, pos);
}
