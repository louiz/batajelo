#ifndef HUD_MESSAGE_HPP_INCLUDED
#define HUD_MESSAGE_HPP_INCLUDED

#include <utils/time.hpp>

#include <string>

class HudMessage
{
public:
  HudMessage(std::string&& text, const utils::Duration& timeout);
  ~HudMessage() = default;
  void set_timeout(const utils::Duration& timeout);
  void update(const utils::Duration& dt);
  utils::Duration get_timeout() const;
  const std::string& get_text() const;

private:
  const std::string text;
  utils::Duration timeout;

  HudMessage(const HudMessage&) = delete;
  HudMessage(HudMessage&&) = delete;
  HudMessage& operator=(const HudMessage&) = delete;
  HudMessage& operator=(HudMessage&&) = delete;
};

#endif /* HUD_MESSAGE_HPP_INCLUDED */
