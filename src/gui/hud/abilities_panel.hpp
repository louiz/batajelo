#ifndef ABILITIES_PANEL_HPP_INCLUDED
#define ABILITIES_PANEL_HPP_INCLUDED

#include <world/entity.hpp>
#include <world/abilities.hpp>
#include <gui/hud/gui_ability.hpp>

#include <map>

class GameClient;

class AbilitiesPanel
{
public:
  AbilitiesPanel(GameClient* game);
  ~AbilitiesPanel() = default;

  const GuiAbility* get(const AbilityType& type) const;

private:
  std::map<AbilityType, GuiAbility> gui_abilities;

  AbilitiesPanel(const AbilitiesPanel&) = delete;
  AbilitiesPanel(AbilitiesPanel&&) = delete;
  AbilitiesPanel& operator=(const AbilitiesPanel&) = delete;
  AbilitiesPanel& operator=(AbilitiesPanel&&) = delete;
};

/**
 * Return all the selected entities with the given ability
 */
template <typename T>
std::vector<EntityId> get_selected_entities_with_ability(const GameClient* game);

#endif /* ABILITIES_PANEL_HPP_INCLUDED */
