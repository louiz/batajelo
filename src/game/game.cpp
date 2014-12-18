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


void Game::new_entity_callback(Message* message)
{
  log_debug("Game::new_entity_callback");
  auto srl = message->parse_body_to_protobuf_object<ser::order::NewEntity>();
  if (!srl.IsInitialized())
    {
      log_error("Invalid data received for new entity: " << srl.InitializationErrorString());
      return ;
    }

  Position pos;
  pos.x.raw() = srl.pos().x();
  pos.y.raw() = srl.pos().y();
  this->turn_handler.insert_action(std::bind(&Game::do_new_entity, this, srl.type_id(), pos, srl.team()),
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

void Game::do_new_entity(const EntityType type, const Position& pos, const uint16_t team)
{
  log_debug("Game::do_new_entity");
  this->world.do_new_entity(type, pos, team);
}
