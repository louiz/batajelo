#include <gui/hud/hud.hpp>

Hud::Hud(GraphMap* map, sf::RenderWindow* win, Camera* camera):
  minimap(win, map, camera),
  win(win)
{
}

Hud::~Hud()
{
}

void Hud::draw(Camera* camera)
{
  this->minimap.draw();
}

bool Hud::handle_event(const sf::Event& event)
{
  if (this->minimap.handle_event(event) == true)
    return true;
  return false;
}
