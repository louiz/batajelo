#include <gui/hud/hud_message.hpp>

HudMessage::HudMessage(std::string&& text, const utils::Duration& timeout):
  text(std::move(text)),
  timeout(timeout)
{
}

void HudMessage::update(const utils::Duration& dt)
{
  this->timeout -= dt;
}

utils::Duration HudMessage::get_timeout() const
{
  return this->timeout;
}

const std::string& HudMessage::get_text() const
{
  return this->text;
}
