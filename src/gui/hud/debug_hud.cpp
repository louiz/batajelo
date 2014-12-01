#include <gui/hud/debug_hud.hpp>
#include <game/game_client.hpp>

DebugHud::DebugHud(GameClient* game_client, Screen* screen):
  ScreenElement(screen),
  game_client(game_client)
{
  if (!this->font.loadFromFile("/usr/share/fonts/dejavu/DejaVuSans.ttf"))
    abort();
}

DebugHud::~DebugHud()
{
}

void DebugHud::draw()
{
  Camera* camera = &this->game_client->get_camera();
  const sf::Vector2u win = camera->get_win_size();
  sf::RectangleShape hline(sf::Vector2f(win.y, 1));
  hline.rotate(90);
  sf::RectangleShape vline(sf::Vector2f(win.x, 1));

  sf::Text nb;
  nb.setCharacterSize(10);
  nb.setFont(this->font);

  // // World pixels
  unsigned int x = 0;
  // while (x <= win.x + camera->x)
  //   {
  //     if (x - camera->x >= 0)
  //       {
  //         nb.setPosition(x - camera->x, 0);
  //         nb.setString(std::to_string(x));
  //         camera->draw(nb);
  //         hline.setPosition(x - camera->x, 0);
  //         camera->draw(hline);
  //       }
  //     x += 100;
  //   }
  unsigned int y = 0;
  // while (y <= win.y + camera->y)
  //   {
  //     if (y - camera->y >= 0)
  //       {
  //         nb.setPosition(0, y - camera->y);
  //         nb.setString(std::to_string(y));
  //         camera->draw(nb);
  //         vline.setPosition(0, y - camera->y);
  //         camera->draw(vline);
  //       }
  //     y += 100;
  //   }

  // // Camera pixels
  // hline.setFillColor(sf::Color::Blue);
  // vline.setFillColor(sf::Color::Blue);
  // x = 0;
  // y = 0;
  // nb.setColor(sf::Color::Blue);
  // while (y <= win.y)
  //   {
  //     nb.setPosition(0, y);
  //     nb.setString(std::to_string(y));
  //     camera->draw(nb);
  //     vline.setPosition(0, y);
  //     camera->draw(vline);
  //     y += 100;
  //   }
  // while (x <= win.x)
  //   {
  //     nb.setPosition(x, 0);
  //     nb.setString(std::to_string(x));
  //     camera->draw(nb);
  //     hline.setPosition(x, 0);
  //     camera->draw(hline);
  //     x += 100;
  //   }

  // World position
  hline.setFillColor(sf::Color::Magenta);
  vline.setFillColor(sf::Color::Magenta);
  x = 0;
  y = 0;
  nb.setColor(sf::Color::Magenta);
  x = 0;
  while (x * TILE_WIDTH <= win.x + camera->x)
    {
      if (x * TILE_WIDTH - camera->x >= 0)
        {
          nb.setPosition(x * TILE_WIDTH - camera->x, 0);
          nb.setString(std::to_string(x * 100));
          camera->draw(nb);
          hline.setPosition(x * TILE_WIDTH - camera->x, 0);
          camera->draw(hline);
        }
      x++;
    }
  y = 0;
  while (y * TILE_HEIGHT <= win.y + camera->y)
    {
      if (y * TILE_HEIGHT - camera->y >= 0)
        {
          nb.setPosition(0, y * TILE_HEIGHT - camera->y + TILE_TOP_OFFSET);
          nb.setString(std::to_string(y * 100));
          camera->draw(nb);
          vline.setPosition(0, y * TILE_HEIGHT - camera->y + TILE_TOP_OFFSET);
          camera->draw(vline);
        }
      y++;
    }

  for (const auto& line: this->text_lines)
    camera->draw(line);
  this->text_lines.clear();
}

void DebugHud::update(const Duration& dt)
{

}

bool DebugHud::handle_event(const sf::Event& event)
{
  return false;
}

void DebugHud::add_debug_line(const std::string& line, const sf::Color& color)
{
  sf::Text txt;
  txt.setFont(this->font);
  txt.setString(line);
  txt.setColor(color);
  txt.setCharacterSize(9);
  txt.setPosition(0, this->text_lines.size() * 9);
  this->text_lines.emplace_back(std::move(txt));
}

