#include <SFML/Graphics.hpp>

#include <gui/hud/abilities_panel.hpp>
#include <game/game_client.hpp>
#include <logging/logging.hpp>

#include <world/entity.hpp>
#include <world/abilities.hpp>
#include <world/abilities/blink.hpp>
#include <world/abilities/attack.hpp>
#include <world/abilities/phase.hpp>
#include <world/abilities/emp.hpp>

#include <utility>

AbilitiesPanel::AbilitiesPanel(GameClient* game)
{
  // Define the Blink button
  auto& blink = this->gui_abilities[AbilityType::Blink];
  blink.left_click = {
    [game](const Position& pos)
    {
      auto ids = get_selected_entities_with_ability<Blink>(game);
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

  // The auto-attack button
  auto& atk = this->gui_abilities[AbilityType::Attack];
  atk.left_click = {
    [game](const Position& pos)
    {
      auto ids = get_selected_entities_with_ability<Attack>(game);
      if (ids.empty())
        return true;
      const bool queue = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));

      const Entity* target = game->get_camera().get_entity_under_mouse();

      log_debug("attacking with " << ids.size() << " entities");
      return game->action_cast(ids, pos, AbilityType::Attack, queue);
    },
    [](const sf::Vector2i&)
    {
      return cursor::Build;
    }
  };

  // The phase button
  auto& phase = this->gui_abilities[AbilityType::Phase];
  phase.callback = [game]()
    {
      auto ids = get_selected_entities_with_ability<Phase>(game);
      if (ids.empty())
        return true;
      const bool queue = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
      log_debug("Activating phase on " << ids.size() << " entities");
      return game->action_cast(ids, Position::invalid, AbilityType::Phase,
                               queue);
    };

  // Define the Emp button
  auto& emp = this->gui_abilities[AbilityType::Emp];
  emp.left_click = {
    [game](const Position& pos)
    {
      auto ids = get_selected_entities_with_ability<Emp>(game);
      if (ids.empty())
        return true;
      const bool queue = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                          sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
      return game->action_cast({ids[0]}, pos, AbilityType::Emp, queue);
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

template <typename T>
std::vector<EntityId> get_selected_entities_with_ability(const GameClient* game)
{
  std::vector<EntityId> ids;
  const auto& selection = game->get_selection();

  for (const auto& entity: selection.get_entities())
    {
      Abilities* abilities = entity->get<Abilities>();
      if (abilities)
        {
          Ability* ability = abilities->find(T::ability_type);
          if (ability)
            {
              // Check cooldown, mana, etc etc
              Entity* e = const_cast<Entity*>(entity);
              ids.push_back(e->get_id());
            }
        }
    }
  return ids;
}
