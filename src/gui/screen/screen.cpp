#include <gui/screen/screen.hpp>

Screen::Screen(ClientWorld* world, GraphMap* map, sf::RenderWindow* win):
  win(win),
  camera(world, map, win),
  hud(map, win, &camera)
{
}

Screen::~Screen()
{
}

void Screen::draw()
{
  this->camera.draw();
  this->hud.draw(&this->camera);
}

void Screen::update(const Duration& dt)
{
  this->camera.update(dt);
}

void Screen::handle_event(const sf::Event& event)
{
  this->camera.handle_event(event);
  this->hud.handle_event(event);
}
