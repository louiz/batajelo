#include <gui/screen/screen.hpp>

Screen::Screen(ClientWorld* world, GraphMap* map, sf::RenderWindow* win):
  win(win),
  camera(world, map, win),
  hud(map, world, win, &camera)
{
}

Screen::~Screen()
{
}

void Screen::draw()
{
  this->camera.draw(this);
  this->hud.draw(&this->camera);
}

void Screen::update(const Duration& dt)
{
  this->camera.update(dt);
}

void Screen::handle_event(const sf::Event& event)
{
  if (this->hud.handle_event(event) == false)
    this->camera.handle_event(event);
}

bool Screen::is_entity_hovered(const Entity* entity) const
{
  return this->hud.is_entity_hovered(entity);
}
