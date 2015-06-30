#include <gui/dash_cursor.hpp>
#include <world/entity.hpp>
#include <game/game_client.hpp>
#include <gui/hud/abilities_panel.hpp>
#include <world/abilities.hpp>
#include <world/location.hpp>
#include <world/abilities/dash.hpp>
#include <world/works/concentrate_work.hpp>

#include <algorithm>

#include <logging/logging.hpp>

cursor::type draw_dash_concentrate_cursor(const sf::Vector2i& mouse_position, GameClient* game)
{
  log_debug("dash cursor position: " << mouse_position.x << "." << mouse_position.y);
  auto ids = get_selected_entities_with_ability<Dash>(game);
  if (ids.empty())
    return cursor::Build;
  Entity* dashing_entity = game->get_world().get_entity_by_id(ids[0]);
  auto abilities = dashing_entity->get<Abilities>();
  auto dash = static_cast<Dash*>(abilities->find(AbilityType::Dash));
  assert(dash);

  const Position world_pos = game->get_camera().screen_to_world_position(
      mouse_position.x, mouse_position.y);
  Location* location = dashing_entity->get<Location>();
  assert(location);

  // Draw a circle at the potential destination
  Vec2 dash_vector = world_pos - location->position();
  if (dash_vector.length() > dash->get_max_distance())
    dash_vector.set_length(dash->get_max_distance());

  Position destination = location->position() + dash_vector;
  auto radius = 30;
  sf::CircleShape destination_circle(radius);
  destination_circle.setOrigin(radius, radius);
  destination_circle.setFillColor({255, 0, 0, 200});
  destination_circle.scale(1.f, 0.666f);
  auto screen_pos = game->get_camera().world_to_screen_position(destination);
  destination_circle.setPosition(screen_pos.x, screen_pos.y);
  game->get_camera().draw(destination_circle);

  // draw a load bar next to the cursor
  EnergyBar bar{{255, 0, 255},
                {255, 255, 0},
                  {100, 10},
                    2, 0};
  int current_val = 0;
  auto concentrate = dynamic_cast<ConcentrateWork*>(dashing_entity->get_current_work());
  if (concentrate)
    {
      auto task = static_cast<ConcentrateTask*>(concentrate->get_task());
      if (task)
        current_val = std::min(task->value().to_int(), dash->get_max_concentration().to_int());
    }
  sf::Transform transform;
  transform.translate(mouse_position.x,
                      mouse_position.y);
  game->get_camera().draw_energy_bar(bar, dash->get_max_concentration().to_int(), current_val,
                                     transform);
  return cursor::Build;
}
