#ifndef INFO_HUD_HPP_INCLUDED
#define INFO_HUD_HPP_INCLUDED

#include <gui/hud/hud_message.hpp>

#include <deque>

class InfoHud
{
public:
  InfoHud();
  ~InfoHud() = default;
  void add_message(std::string&& text);
  void update(const utils::Duration& dt);
  const std::deque<HudMessage>& get_messages() const;

private:
  std::deque<HudMessage> messages;
  static const utils::Duration message_timeout;
  static const size_t max_size;

  InfoHud(const InfoHud&) = delete;
  InfoHud(InfoHud&&) = delete;
  InfoHud& operator=(const InfoHud&) = delete;
  InfoHud& operator=(InfoHud&&) = delete;
};

#endif /* INFO_HUD_HPP_INCLUDED */
