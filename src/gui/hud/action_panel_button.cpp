#include <gui/hud/action_panel_button.hpp>
#include <gui/common.hpp>
#include <gui/hud/action_panel.hpp>
#include <logging/logging.hpp>

ActionPanelButton::ActionPanelButton(const std::string& image_filename, const t_action_panel_button_callback callback, const std::size_t position, const t_left_click left_click):
  callback(callback),
  position(position),
  left_click(left_click)
{
  const std::string image_directory("./data/images/actions/");
  const std::string filename = image_directory + image_filename;
  log_error("Creating button with image: " << filename);
  if (!this->texture.loadFromFile(filename.c_str()))
    throw GraphInitError();
  this->sprite.setTexture(this->texture);
  this->x = ACTION_PANEL_X + ((ACTION_SQUARE_SIZE + ACTION_SPACE_BETWEEN_SQUARES) * (position % ACTION_SQUARES_PER_LINE));
  this->y = ACTION_PANEL_Y + ((ACTION_SQUARE_SIZE + ACTION_SPACE_BETWEEN_LINES) * (position / ACTION_SQUARES_PER_LINE));
  log_error(position / ACTION_SQUARES_PER_LINE);
  this->sprite.setPosition(this->x, this->y);
}

ActionPanelButton::~ActionPanelButton()
{
}

void ActionPanelButton::draw(sf::RenderWindow* win)
{
  win->draw(this->sprite);
}

void ActionPanelButton::on_clicked()
{
  if (!this->callback)
    {
      log_warning("Empty action panel button callback");
    }
  else
    {
      this->callback(this->left_click);
    }
}
