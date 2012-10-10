#include <boost/bind.hpp>

#include <gui/hud/hud.hpp>
#include <world/entity.hpp>

Hud::Hud(GraphMap* map, ClientWorld* world, sf::RenderWindow* win, Camera* camera, Screen* screen, ClientMod& mod):
  minimap(win, map, camera),
  win(win),
  world(world),
  selection_panel(win, world->get_selection_ptr()),
  action_panel(win, screen, world->get_selection_ptr(), mod)
{
  // Install a callback on the selection that will reset the action_panel
  // whenever the selection is modified.
  world->add_selection_change_callback(boost::bind(&ActionPanel::reset_all_tables, &this->action_panel));
  this->hud_texture.loadFromFile("./data/images/hud.png");
  this->hud_sprite.setTexture(this->hud_texture);
  this->hud_sprite.setPosition(0, win->getSize().y - HUD_HEIGHT);
}

Hud::~Hud()
{
}

void Hud::draw(Camera* camera)
{
  this->win->draw(this->hud_sprite);
  this->minimap.draw();
  // if (this->world->get_selection().size() > 1)
  this->selection_panel.draw();
  this->action_panel.draw();
}

bool Hud::handle_event(const sf::Event& event)
{
  if (this->minimap.handle_event(event) == true)
    return true;
  if (this->selection_panel.handle_event(event, this->world) == true)
    return true;
  if (this->action_panel.handle_event(event) == true)
    return true;
  return false;
}

bool Hud::is_entity_hovered(const Entity* entity) const
{
  return this->selection_panel.is_entity_hovered(entity);
}

void Hud::reset_left_click_action()
{
  this->action_panel.reset_all_tables();
}
