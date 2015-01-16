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
  if (!this->font.loadFromFile("/usr/share/fonts/dejavu/DejaVuSans.ttf"))
    abort();
}

Hud::~Hud()
{
}

void Hud::draw()
{
  this->screen->window().draw(this->hud_sprite);
  using namespace std::chrono_literals;

  sf::Text txt;
  txt.setFont(this->font);
  txt.setCharacterSize(20);
  txt.setColor(sf::Color::Red);
  unsigned int y = 300;
  for (const auto& message: this->info_hud.get_messages())
    {
      txt.setPosition(10, y);
      txt.setString(message.get_text());
      if (message.get_timeout() > 1s)
        txt.setColor(sf::Color::Red);
      else
        {
          const float ratio = static_cast<float>(message.get_timeout().count()) /
            std::chrono::duration_cast<utils::Duration>(1s).count();
          txt.setColor({255, 0, 0, static_cast<uint8_t>(255*ratio)});
        }
      this->screen->window().draw(txt);
      y += 20;
    }
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

void Hud::add_info_message(std::string&& text)
{
  this->info_hud.add_message(std::move(text));
}

void Hud::update(const utils::Duration& dt)
{
  this->info_hud.update(dt);
}

bool Hud::is_entity_hovered(const Entity* entity) const
{
  // return this->selection_panel.is_entity_hovered(entity);
  return false;
}
