#include <logging/logging.hpp>
#include <gui/screen/screen.hpp>
#include <gui/common.hpp>

Screen::Screen():
  win{sf::VideoMode(1920, 1080), "Bata", sf::Style::Default, sf::ContextSettings{}},
  current_cursor_type(cursor::Normal)
{
  // world->camera = &this->camera;
  this->cursor_textures.resize(cursor::size);
  const std::string data_path("./data/images/");
  for (int i = 0; i < cursor::size; ++i)
    {
      const std::string filename = data_path + cursor::filenames[i];
      if (!this->cursor_textures[i].loadFromFile(filename.c_str()))
        throw GraphInitError();
    }
  this->cursor_sprite.setTexture(this->cursor_textures[cursor::Normal]);
  // this->building_textures = mod.get_building_textures();
  this->win.setPosition(sf::Vector2i(0, 0));
  // this->win.setMouseCursorVisible(false);
}

Screen::~Screen()
{
}

sf::RenderWindow& Screen::window()
{
  return this->win;
}

void Screen::draw()
{
  std::for_each(this->screen_elements.begin(), this->screen_elements.end(),
                [](const auto element)
                {
                  element->draw();
                });
  this->draw_mouse_cursor();
}

void Screen::update(const utils::Duration& dt)
{
  for (const auto& element: this->screen_elements)
    element->update(dt);
}

void Screen::handle_event(const sf::Event& event)
{
  for (const auto& element: this->screen_elements)
    if (element->handle_event(event) == true)
      break;                  // The element “ate” the event
}

void Screen::add_element(ScreenElement* element, const int priority)
{
  element->set_priority(priority);

  const auto it = std::find_if(this->screen_elements.begin(), this->screen_elements.end(),
                               [element](const auto elem)
                               {
                                 return elem->get_priority() > element->get_priority();
                               });
  this->screen_elements.insert(it, element);
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
  const sf::Vector2i pos = this->get_mouse_position();
  // this->debug_hud.add_debug_line("Cursor position: " +
  //                                std::to_string(pos.x) + ", " +
  //                                std::to_string(pos.y));
  if (this->on_left_click.cursor_callback != 0)
    this->set_cursor_type(this->on_left_click.cursor_callback(static_cast<uint>(pos.x),
                                                              static_cast<uint>(pos.y),
                                                              on_left_click.id));
  else
    this->set_cursor_type(cursor::Normal);
  this->cursor_sprite.setPosition(static_cast<float>(pos.x) - 42.f,
                                  static_cast<float>(pos.y) - 42.f);
  this->win.draw(this->cursor_sprite);
}

void Screen::reset_left_click_action()
{
  t_left_click null = {0, 0, 0};
  this->set_left_click_callback(null);
  // this->hud.reset_left_click_action();
}

void Screen::set_left_click_callback(const t_left_click left_click)
{
  this->on_left_click = left_click;
}

cursor::type Screen::draw_move_cursor(const uint, const uint, const std::size_t)
{
  return cursor::Move;
}

sf::Vector2i Screen::get_mouse_position() const
{
  return sf::Mouse::getPosition(this->win);
}

const sf::Vector2u Screen::get_window_size() const
{
  return this->win.getSize();
}
