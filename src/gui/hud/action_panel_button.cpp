#include <gui/hud/action_panel_button.hpp>
#include <gui/common.hpp>
#include <gui/hud/action_panel.hpp>

ActionPanelButton::ActionPanelButton(const std::string& image_filename, const t_action_panel_button_callback callback, const std::size_t position, const t_left_click_callback left_click_callback, const cursor::type cursor_type):
  callback(callback),
  position(position),
  left_click_callback(left_click_callback),
  cursor_type(cursor_type)
{
  if (!this->texture.loadFromFile(image_filename.c_str()))
    throw GraphInitError();
  this->sprite.setTexture(this->texture);
  this->x = ACTION_PANEL_X + ((ACTION_SQUARE_SIZE + ACTION_SPACE_BETWEEN_SQUARES) * (position % ACTION_SQUARES_PER_LINE));
  this->y = ACTION_PANEL_Y + ((ACTION_SQUARE_SIZE + ACTION_SPACE_BETWEEN_LINES) * (position / ACTION_SQUARES_PER_LINE));
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
  this->callback(this->left_click_callback, cursor_type);
}
