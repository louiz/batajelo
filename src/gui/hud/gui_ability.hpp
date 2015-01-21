#ifndef GUI_ABILITY_HPP_INCLUDED
#define GUI_ABILITY_HPP_INCLUDED

/**
 * Contains all the information about an ability, for the GUI part. For
 * example the icone to use, the activate callback (it can execute the
 * ability directly, or change the LeftClick structure, which only involves
 * the GUI)
 */

#include <gui/screen/left_click.hpp>

class GuiAbility
{
public:
  GuiAbility() = default;
  ~GuiAbility() = default;

  LeftClick left_click;

private:
  GuiAbility(const GuiAbility&) = delete;
  GuiAbility(GuiAbility&&) = delete;
  GuiAbility& operator=(const GuiAbility&) = delete;
  GuiAbility& operator=(GuiAbility&&) = delete;
};

#endif /* GUI_ABILITY_HPP_INCLUDED */
