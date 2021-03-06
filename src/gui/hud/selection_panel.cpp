#include <logging/logging.hpp>
#include <gui/hud/selection_panel.hpp>
#include <game/game_client.hpp>
#include <gui/screen/screen.hpp>
#include <assets/assets_manager.hpp>

SelectionPanel::SelectionPanel(GameClient* game, Screen* screen, const Selection* selection):
  ScreenElement(screen),
  current_tab(0),
  game(game),
  selection(selection)
{
}

SelectionPanel::~SelectionPanel()
{
}

void SelectionPanel::draw()
{
  // Check if we are not on a tab that is empty because of the selection
  // being too small
  const std::size_t square_per_tab = SQUARES_PER_LINE * LINES_PER_TAB;
  const sf::Vector2u win_size = this->screen->get_window_size();

  if (this->current_tab * square_per_tab >= this->selection->size())
    this->current_tab = 0;
  auto entities = this->selection->get_entities();
  sf::Texture texture;
  texture.loadFromFile(AssetsManager::full_name("images/unit50.png"));
  sf::Sprite sprite;
  sprite.setTexture(texture);
  uint x = PANEL_X;
  uint y = PANEL_Y;
  int tab = -1;
  std::size_t n = 0;
  for (auto it = entities.begin(); it != entities.end(); ++it)
    {
      if (n % (SQUARES_PER_LINE * LINES_PER_TAB) == 0)
        {
          tab++;
          this->draw_tab(tab);
        }
      n++;
      if (tab == this->current_tab)
        {
          sprite.setPosition(x, win_size.y - y);
          this->screen->window().draw(sprite);
          if (n % SQUARES_PER_LINE == 0)
            {
              x = PANEL_X;
              y -= SQUARE_SIZE + SPACE_BETWEEN_LINES;
            }
          else
            x += SQUARE_SIZE + SPACE_BETWEEN_SQUARES;
        }
    }
}

void SelectionPanel::draw_tab(const std::size_t tab_number)
{
  const std::size_t n = tab_number+1;
  sf::RectangleShape tab(sf::Vector2f(TAB_WIDTH, TAB_HEIGHT));
  tab.setOutlineColor(sf::Color::Black);
  if (this->current_tab == tab_number)
    tab.setFillColor(sf::Color(120, 120, 12));
  else
    tab.setFillColor(sf::Color(120, 120, 120));
  tab.setOutlineThickness(1);
  tab.setPosition(TABS_X, this->screen->get_window_size().y - TABS_Y + tab_number * (TAB_HEIGHT + SPACE_BETWEEN_TABS));
  this->screen->window().draw(tab);
}

bool SelectionPanel::handle_event(const sf::Event& event)
{
  const sf::Vector2u win_size = this->screen->get_window_size();
  // Check if we clicked on a tab
  if ((event.mouseButton.x < (TABS_X + TAB_WIDTH)) &&
      (event.mouseButton.x > (TABS_X)))
    {
      if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
          const int y = event.mouseButton.y - (win_size.y - TABS_Y);
          if (y >= 0)
            {
              int n = y / (TAB_HEIGHT + SPACE_BETWEEN_TABS);
              // Check if the tab we try to select does exist.
              const std::size_t square_per_tab = SQUARES_PER_LINE * LINES_PER_TAB;
              if (this->current_tab * square_per_tab >= this->selection->size())
                return false;
              if (y % (TAB_HEIGHT + SPACE_BETWEEN_TABS) <= TAB_HEIGHT)
                {
                  this->current_tab = n;
                  return true;
                }
            }
        }
    }
  // Check if we are over the entity squares
  if (event.type == sf::Event::MouseButtonPressed)
    if ((event.mouseButton.x > PANEL_X) &&
        (event.mouseButton.x < (PANEL_X + (SPACE_BETWEEN_SQUARES + SQUARE_SIZE) * SQUARES_PER_LINE - SPACE_BETWEEN_SQUARES)) &&
        (event.mouseButton.y > win_size.y - PANEL_Y))
      {
        log_error("event on panel");
        const Entity* entity = this->get_entity_under_mouse();
        if (!entity)
          return false;
        switch (event.mouseButton.button)
          {
          case sf::Mouse::Left:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
              {
                log_error(this->selection->size());
                this->game->unselect_entity(entity);
                log_error(this->selection->size());
              }
            else
              {
                this->game->clear_selection();
                this->game->select_entity(entity);
              }
            break;
          }
        return true;
      }
  return false;
}

const Entity* SelectionPanel::get_entity_under_mouse() const
{
  const sf::Vector2i pos = this->screen->get_mouse_position();
  const sf::Vector2u win_size = this->screen->get_window_size();

  const int posx = pos.x - PANEL_X;
  const int posy = pos.y - (win_size.y - PANEL_Y);

  const int x = posx / (SQUARE_SIZE + SPACE_BETWEEN_SQUARES);
  const int y = posy / (SQUARE_SIZE + SPACE_BETWEEN_LINES);

  int n = (this->current_tab * (SQUARES_PER_LINE * LINES_PER_TAB)) +
    y * SQUARES_PER_LINE + x;
  log_error(n);

  const auto& entities = this->selection->get_entities();
  int nn = 0;
    for (auto it = entities.begin(); it != entities.end(); ++it)
      {
        if (nn == n)
          return *it;
        ++nn;
      }
  return 0;
}

bool SelectionPanel::is_entity_hovered(const Entity* entity) const
{
  const auto& entities = this->selection->get_entities();
  const sf::Vector2i pos = this->screen->get_mouse_position();

  int tab = -1;
  std::size_t n = 0;
  for (auto it = entities.begin(); it != entities.end(); ++it)
    {
      if (n % (SQUARES_PER_LINE * LINES_PER_TAB) == 0)
        tab++;
      if (tab == this->current_tab &&  *it == entity)
        {
          n = n - (tab * (SQUARES_PER_LINE * LINES_PER_TAB));
          const std::size_t x = n % SQUARES_PER_LINE;
          const std::size_t y = n / SQUARES_PER_LINE;
          const int posx = pos.x - PANEL_X;
          const int posy = pos.y - (this->screen->get_window_size().y - PANEL_Y);
          if (((posx >= (SQUARE_SIZE + SPACE_BETWEEN_SQUARES) * x) && (posx < (SQUARE_SIZE + SPACE_BETWEEN_SQUARES) * x + SQUARE_SIZE)) && ((posy >= (SQUARE_SIZE + SPACE_BETWEEN_TABS) * y) && (posy < (SQUARE_SIZE + SPACE_BETWEEN_TABS) * y + SQUARE_SIZE)))
            return true;
        }
      n++;
    }
  return false;
}

void SelectionPanel::update(const utils::Duration& dt)
{
}
