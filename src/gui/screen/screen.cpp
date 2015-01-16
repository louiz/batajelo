#include <logging/logging.hpp>
#include <gui/screen/screen.hpp>
#include <gui/common.hpp>

LeftClick LeftClick::null = {nullptr, nullptr};

Screen::Screen():
  win{sf::VideoMode(1920, 1080), "Bata", sf::Style::Default, sf::ContextSettings{}},
  current_cursor_type(cursor::Normal)
{
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

  if (this->left_click.cursor_callback)
    this->set_cursor_type(this->left_click.cursor_callback(pos));
  else
    this->set_cursor_type(cursor::Normal);
  this->cursor_sprite.setPosition(static_cast<float>(pos.x) - 42.f,
                                  static_cast<float>(pos.y) - 42.f);
  this->win.draw(this->cursor_sprite);
}

void Screen::reset_left_click()
{
  this->set_left_click(LeftClick::null);
}

void Screen::set_left_click(const LeftClick& left_click)
{
  this->left_click = left_click;
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
