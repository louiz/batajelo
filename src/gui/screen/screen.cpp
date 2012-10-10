#include <gui/screen/screen.hpp>
#include <gui/common.hpp>

Screen::Screen(ClientWorld* world, GraphMap* map, sf::RenderWindow* win, ClientMod& mod):
  win(win),
  camera(world, map, win, this),
  world(world),
  hud(map, world, win, &camera, this, mod),
  current_cursor_type(cursor::Normal)
{
  this->cursor_textures.resize(cursor::size);
  const std::string data_path("./data/images/");
  for (int i = 0; i < cursor::size; ++i)
    {
      const std::string filename = data_path + cursor::filenames[i];
      if (!this->cursor_textures[i].loadFromFile(filename.c_str()))
        throw GraphInitError();
    }
  this->cursor_sprite.setTexture(this->cursor_textures[cursor::Normal]);
  this->building_sprites = mod.get_building_sprites();
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
  if (this->current_cursor_type == type)
    return ; // Just avoid to call setTexture() ~60 times per seconds
  this->current_cursor_type = type;
  this->cursor_sprite.setTexture(this->cursor_textures[type]);
}

void Screen::draw_mouse_cursor()
{
  const sf::Vector2i pos = sf::Mouse::getPosition(*this->win);
  if (this->on_left_click.cursor_callback != 0)
    this->set_cursor_type(this->on_left_click.cursor_callback(static_cast<uint>(pos.x),
                                                              static_cast<uint>(pos.y),
                                                              on_left_click.id));
  else
    this->set_cursor_type(cursor::Normal);
  this->cursor_sprite.setPosition(static_cast<float>(pos.x) - 42.f,
                                  static_cast<float>(pos.y) - 42.f);
  this->win->draw(this->cursor_sprite);
}

void Screen::reset_left_click_action()
{
  t_left_click null = {0, 0, 0};
  this->set_left_click_callback(null);
  this->hud.reset_left_click_action();
}

void Screen::set_left_click_callback(const t_left_click left_click)
{
  this->on_left_click = left_click;
}

cursor::type Screen::draw_build_cursor(const uint x, const uint y, const std::size_t id)
{
  log_error("id: " << id);
  sf::Sprite& sprite = this->building_sprites[id]->get_cursor_sprite();
  sprite.setPosition(static_cast<float>(x) - 42.f,
                     static_cast<float>(y) - 42.f);
  this->win->draw(sprite);
  return cursor::Build;
}

cursor::type Screen::draw_move_cursor(const uint, const uint, const std::size_t)
{
  return cursor::Move;
}
