#include <SFML/Graphics.hpp>

#include <gui/hud/abilities_panel.hpp>
#include <game/game_client.hpp>
#include <logging/logging.hpp>

#include <world/entity.hpp>
#include <world/abilities.hpp>
#include <world/abilities/blink.hpp>

#include <utility>

AbilitiesPanel::AbilitiesPanel(GameClient* game)
{
  auto& blink = this->gui_abilities[AbilityType::Blink];
  blink.left_click = {
    [game](const Position& pos)
    {
      std::vector<EntityId> ids;
      const auto& selection = game->get_selection();
      if (selection.is_empty())
        return true;
      // Find all entities with a blink ability
      for (const auto& entity: selection.get_entities())
        {
          Abilities* abilities = entity->get<Abilities>();
          if (abilities)
            {
              Ability* ability = abilities->find(AbilityType::Blink);
              if (ability)
                {
                  Blink* blink = static_cast<Blink*>(ability);
                  // Check cooldown, mana, etc etc etc
                  Entity* e = const_cast<Entity*>(entity);
                  ids.push_back(e->get_id());
                }
            }
        }
      if (ids.empty())
        return true;
      const bool queue = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
      return game->action_cast(ids, pos, AbilityType::Blink, queue);
    },
    [](const sf::Vector2i&)
    {
      return cursor::Move;
    }
  };
}

const GuiAbility* AbilitiesPanel::get(const AbilityType& type) const
{
  const auto it = this->gui_abilities.find(type);
  if (it == this->gui_abilities.end())
    return nullptr;
  return &it->second;
}
