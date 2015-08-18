#ifndef SOUNDS_HANDLER_HPP_INCLUDED
#define SOUNDS_HANDLER_HPP_INCLUDED

#include <SFML/Audio.hpp>

#include <game/sound.hpp>

#include <random>
#include <vector>
#include <chrono>
#include <map>

enum class SoundType
{
  DefaultOk,
  ProjectileLaunch,
  ProjectileImpact,
  BlinkStart,
  EmpStart,
  EmpExplode,
  Concentrate,
  Dash,

  count,
};

/**
 * Keep a list of loaded SoundBuffers, and spawn (playing) Sound objects on demand.
 */

class SoundsHandler
{
public:
  SoundsHandler();
  ~SoundsHandler() = default;
  /**
   * Spawns a Sound, and play it immediately. Keep it in the sounds buffer.
   */
  void play(const SoundType& type, const bool limit=false, const float volume=100.f);

private:
  void load(const std::string& filename, const SoundType& type);
  std::map<SoundType, std::vector<sf::SoundBuffer>> buffers;

  /**
   * Array of sounds in memory. Each time we play a new sound, we look in
   * that array for a Sound for which getStatus() is stopped, and we use
   * that Sound to play the new sound. If max_sounds are all playing, the
   * new sounds is not started.
   */
  static constexpr std::size_t max_sounds = 64;
  std::array<Sound, max_sounds> sounds;

  std::map<SoundType, std::chrono::steady_clock::time_point> timeouts;

  /**
   * Random stuf, to choose random sounds among a list
   */
  std::random_device rd;
  std::mt19937 random_engine;

  SoundsHandler(const SoundsHandler&) = delete;
  SoundsHandler(SoundsHandler&&) = delete;
  SoundsHandler& operator=(const SoundsHandler&) = delete;
  SoundsHandler& operator=(SoundsHandler&&) = delete;
};

#endif /* SOUNDS_HANDLER_HPP_INCLUDED */
