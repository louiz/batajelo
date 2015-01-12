#include <gui/hud/info_hud.hpp>
#include <algorithm>

using namespace std::chrono_literals;

const utils::Duration InfoHud::message_timeout{4s};
const size_t InfoHud::max_size(8);

InfoHud::InfoHud()
{
}

void InfoHud::update(const utils::Duration& dt)
{
  for (auto& message: this->messages)
    message.update(dt);

  // Remove all expired messages. They can only be at the front because the
  // queue is sorted
  auto it = this->messages.begin();
  while (it != this->messages.end() && it->get_timeout() < 0us)
    {
      this->messages.pop_front();
      it = this->messages.begin();
    }
}

void InfoHud::add_message(std::string&& text)
{
  this->messages.emplace_back(std::move(text), this->message_timeout);
  while (this->messages.size() > this->max_size)
    this->messages.pop_front();
}

const std::deque<HudMessage>& InfoHud::get_messages() const
{
  return this->messages;
}
