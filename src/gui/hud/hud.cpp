#include <gui/screen/screen.hpp>
#include <game/game_client.hpp>
#include <functional>
#include <gui/hud/hud.hpp>
#include <world/entity.hpp>

#include <world/abilities.hpp>
#include <world/abilities/blink.hpp>

#include <gui/hud/abilities_panel.hpp>

#include <logging/logging.hpp>


Hud::Hud(GameClient* game, Screen* screen):
  ScreenElement(screen),
  // minimap(game, screen),
  // selection_panel(game, world->get_selection_ptr()),
  // action_panel(game, screen, world->get_selection_ptr(), mod),
  game(game),
  abilities_panel(game)
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
  static const sf::Color debug_color = sf::Color::Black;

  this->screen->window().draw(this->hud_sprite);
  auto selected_entities = this->game->get_selection().get_entities();
  auto it = selected_entities.begin();
  this->game->get_debug_hud().add_debug_line("======== HUD begin ===========", debug_color);
  this->game->get_debug_hud().add_debug_line(std::to_string(selected_entities.size()) + " entities selected.", debug_color);
  if (it != selected_entities.end())
    {
      this->game->get_debug_hud().add_debug_line("One (or more) unit selected. Abilities: ", debug_color);
      auto entity = *it;
      auto abilities = entity->get<Abilities>();
      for (const auto& ability: abilities->get())
        {
          if (ability)
            this->game->get_debug_hud().add_debug_line(ability->get_name(), debug_color);
        }
    }

  this->game->get_debug_hud().add_debug_line("------------- HUD end -------------", debug_color);

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
  if (event.type == sf::Event::KeyPressed)
    return this->handle_keypress(event);
  return false;
}

bool Hud::handle_keypress(const sf::Event& event)
{
  this->game->get_hud().add_info_message("Received key: " + std::to_string(event.key.code));
  switch (event.key.code)
    {
    case sf::Keyboard::A:
      this->activate_ability(0);
      return true;
    case sf::Keyboard::U:
      this->activate_ability(1);
      return true;
    case sf::Keyboard::I:
      this->activate_ability(2);
      return true;
    case sf::Keyboard::E:
      this->activate_ability(3);
      return true;
    default:
      return false;
    }
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

void Hud::activate_ability(const std::size_t nb)
{
  const Selection& selection = this->game->get_selection();
  if (selection.is_empty())
    this->game->get_hud().add_info_message("No entity selected");
  else
    {
      const auto entity = selection.get_entities().front();
      Abilities* abilities = entity->get<Abilities>();
      if (!abilities)
        this->game->get_hud().add_info_message("Selected entity has no ability.");
      else
        {
          const Ability* ability = abilities->get(nb);
          if (!ability)
            this->game->get_hud().add_info_message("Selected entity has no ability number " + std::to_string(nb));
          else
            {
              this->game->get_hud().add_info_message("Activating ability " + ability->get_name());
              this->game->get_hud().add_info_message("The ability type is " + std::to_string(static_cast<int>(ability->get_type())));
              const GuiAbility* gui_ab = this->abilities_panel.get(ability->get_type());
              assert(gui_ab);
              if (gui_ab->callback)
                gui_ab->callback();
              else
                this->screen->set_left_click(gui_ab->left_click);
            }
        }
    }
  // TODO actually only look in the abilities of actiev entity TYPE, not the
  // first one
}

