#include <gui/screen/screen.hpp>
#include <game/game_client.hpp>
#include <functional>
#include <gui/hud/hud.hpp>
#include <world/entity.hpp>

Hud::Hud(GameClient* game, Screen* screen):
  ScreenElement(screen),
  // minimap(game, screen),
  // selection_panel(game, world->get_selection_ptr()),
  // action_panel(game, screen, world->get_selection_ptr(), mod),
  game(game)
{
  const sf::Vector2u win_size = this->screen->get_window_size();
  // Install a callback on the selection that will reset the action_panel
  // whenever the selection is modified.

  // world->add_selection_change_callback(std::bind(&ActionPanel::reset_all_tables, &this->action_panel));
  this->hud_texture.loadFromFile("./data/images/hud.png");
  this->hud_sprite.setTexture(this->hud_texture);
  this->hud_sprite.setPosition(0, win_size.y - HUD_HEIGHT);
}

Hud::~Hud()
{
}

void Hud::draw()
{
  this->screen->window().draw(this->hud_sprite);
  // this->minimap.draw();
  // if (this->world->get_selection().size() > 1)
  // this->selection_panel.draw();
  // this->action_panel.draw();
}

bool Hud::handle_event(const sf::Event& event)
{
  // if (this->minimap.handle_event(event) == true)
  //   return true;
  // if (this->selection_panel.handle_event(event, this->world) == true)
  //   return true;
  // if (this->action_panel.handle_event(event) == true)
  //   return true;
  return false;
}

void update(const Duration& dt)
{
}

bool Hud::is_entity_hovered(const Entity* entity) const
{
  // return this->selection_panel.is_entity_hovered(entity);
  return false;
}

void Hud::reset_left_click_action()
{
  // this->action_panel.reset_all_tables();
}

void Hud::update(const Duration& dt)
{
}
