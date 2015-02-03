#include <game/sound.hpp>

bool Sound::is_playing() const
{
  auto status = this->sound.getStatus();
  if (status == sf::SoundSource::Status::Playing)
    return true;
  return false;
}

void Sound::play(const sf::SoundBuffer& buffer, const float volume)
{
  this->sound.setBuffer(buffer);
  this->sound.setVolume(volume);
  this->sound.play();
}
