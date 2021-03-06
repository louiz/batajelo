#include <game/game.hpp>
#include <orders.pb.h>
#include <network/message.hpp>

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

  Position pos;
  pos.x.raw() = srl.pos().x();
  pos.y.raw() = srl.pos().y();
  this->turn_handler.insert_action(std::bind(&World::do_new_entity, &this->world, srl.type_id(), pos, srl.team()),
                                   srl.turn());
}

void Game::move_callback(Message* message)
{
  log_debug("Game::move_callback");
  auto srl = message->parse_body_to_protobuf_object<ser::order::Move>();

  std::vector<EntityId> ids;
  for (const auto& id: srl.entity_id())
    ids.push_back(id);

  if (srl.has_pos())
    {
      Position pos;
      pos.x.raw() = srl.mutable_pos()->x();
      pos.y.raw() = srl.mutable_pos()->y();

      this->turn_handler.insert_action(
          std::bind(&World::do_move, &this->world, ids, pos, srl.queue()),
          srl.turn());
    }
  else
    {
      this->turn_handler.insert_action(
          std::bind(&World::do_follow, &this->world, ids, srl.target(), srl.queue()),
          srl.turn());
    }
}

void Game::cast_callback(Message* message)
{
  log_debug("Game::cast_callback");
  auto srl = message->parse_body_to_protobuf_object<ser::order::Cast>();

  std::vector<EntityId> ids;
  for (const auto& id: srl.entity_id())
    ids.push_back(id);

  AbilityType type = static_cast<AbilityType>(srl.type());

  if (srl.has_target())
    {
      this->turn_handler.insert_action(
          std::bind(&World::do_cast_on_target, &this->world, ids, srl.target(), type, srl.queue()),
          srl.turn());
    }
  else if (srl.has_pos())
    {
      Position pos;
      pos.x.raw() = srl.pos().x();
      pos.y.raw() = srl.pos().y();

      this->turn_handler.insert_action(
          std::bind(&World::do_cast_on_pos, &this->world, ids, pos, type, srl.queue()),
          srl.turn());
    }
  else
    {
      this->turn_handler.insert_action(
             std::bind(&World::do_cast, &this->world, ids, type, srl.queue()),
             srl.turn());
    }
}

utils::FloatingSeconds Game::current_world_time() const
{
  return this->turn_handler.get_current_time();
}
