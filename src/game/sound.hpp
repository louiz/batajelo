#ifndef SOUND_HPP_INCLUDED
#define SOUND_HPP_INCLUDED

#include <SFML/Audio.hpp>

/**
 * Todo keep a weak pointer on an entity, or a position and use that
 * position, and the current camera position to sound.setPosition() each
 * tick
 */

class Sound
{
public:
  Sound() = default;
  ~Sound() = default;
  bool is_playing() const;
  /**
   * Start playing the sound using the given buffer.
   */
  void play(const sf::SoundBuffer& buffer);

private:
  sf::Sound sound;

  Sound(const Sound&) = delete;
  Sound(Sound&&) = delete;
  Sound& operator=(const Sound&) = delete;
  Sound& operator=(Sound&&) = delete;
};

#endif /* SOUND_HPP_INCLUDED */
