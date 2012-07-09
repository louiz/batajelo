#include <gui/hud/hud.hpp>

Hud::Hud(GraphMap* map, sf::RenderWindow* win, Camera* camera):
  minimap(win, map, camera),
  win(win)
{
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
}

bool Hud::handle_event(const sf::Event& event)
{
  if (this->minimap.handle_event(event) == true)
    return true;
  return false;
}
