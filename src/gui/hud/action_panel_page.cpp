#include <gui/hud/action_panel_page.hpp>
#include <gui/hud/action_panel.hpp>

ActionPanelPage::ActionPanelPage()
{
  this->buttons.resize(ACTION_SQUARES_PER_LINE * ACTION_NUMBER_OF_LINES, 0);
}

ActionPanelPage::~ActionPanelPage()
{
  for (int i = 0; i < ACTION_SQUARES_PER_LINE * ACTION_NUMBER_OF_LINES; ++i)
    {
      if (this->buttons[i])
        delete this->buttons[i];
    }
}

void ActionPanelPage::add_button(ActionPanelButton* button, std::size_t position)
{
  this->buttons[position] = button;
}

void ActionPanelPage::draw(sf::RenderWindow* win)
{
  for (std::vector<ActionPanelButton*>::iterator it = this->buttons.begin();
       it != this->buttons.end(); ++it)
    {
      if (*it)
        (*it)->draw(win);
    }
}

bool ActionPanelPage::handle_event(const sf::Event& event)
{
  ActionPanelButton* button;
  if (event.type == sf::Event::MouseButtonPressed &&
      event.mouseButton.button == sf::Mouse::Left)
    {
      for (std::vector<ActionPanelButton*>::iterator it = this->buttons.begin();
           it != this->buttons.end(); ++it)
        {
          if (*it)
            {
              button = *it;
              if ((event.mouseButton.x >= button->x) && (event.mouseButton.x < button->x + ACTION_SQUARE_SIZE) && (event.mouseButton.y >= button->y) && (event.mouseButton.y < button->y + ACTION_SQUARE_SIZE))
                {
                  button->on_clicked();
                  return true;
                }
            }
        }
    }
  return false;
}
//   int x;
//   int y = ACTION_PANEL_Y;
//   for (int j = 0; j < ACTION_NUMBER_OF_LINES; ++j)
//     {
//       x = ACTION_PANEL_X;
//       for (int i = 0; i < ACTION_SQUARES_PER_LINE; ++i)
//         {
          
//         }
//     }

// }
