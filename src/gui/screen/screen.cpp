#include <gui/screen/screen.hpp>
#include <gui/common.hpp>

Screen::Screen(ClientWorld* world, GraphMap* map, sf::RenderWindow* win):
  win(win),
  camera(world, map, win, this),
  world(world),
  hud(map, world, win, &camera, this)
{
  this->cursor_textures.resize(cursor::size);
  const std::string data_path("./data/images/");
  for (int i = 0; i < cursor::size; ++i)
    {
      const std::string filename = data_path + cursor::filenames[i];
      if (!this->cursor_textures[i].loadFromFile(filename.c_str()))
        throw GraphInitError();
    }
  this->set_cursor_type(cursor::Normal);
}

Screen::~Screen()
{
}

void Screen::draw()
{
  this->camera.draw();
  this->hud.draw(&this->camera);
  this->draw_mouse_cursor();
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

void Screen::set_cursor_type(const cursor::type type)
{
  this->cursor_sprite.setTexture(this->cursor_textures[type]);
}

void Screen::draw_mouse_cursor()
{
  const sf::Vector2i pos = sf::Mouse::getPosition(*this->win);
  this->cursor_sprite.setPosition(static_cast<float>(pos.x) - 42.f,
                                  static_cast<float>(pos.y) - 42.f);
  this->win->draw(this->cursor_sprite);
}
