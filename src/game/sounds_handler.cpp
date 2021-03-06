#include <game/sounds_handler.hpp>
#include <assets/assets_manager.hpp>
#include <stdexcept>

#include <string>

#include <logging/logging.hpp>

using namespace std::string_literals;
using namespace std::chrono_literals;

using AM = AssetsManager;

SoundsHandler::SoundsHandler():
  rd(),
  random_engine(rd())
{
  this->load(AM::full_name("sounds/gyro/Gyro_move_01.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_02.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_03.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_05.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_06.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_07.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_08.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_10.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_11.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_12.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_13.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_16.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_19.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_25.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_27.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_30.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_31.wav"), SoundType::DefaultOk);
  this->load(AM::full_name("sounds/gyro/Gyro_move_32.wav"), SoundType::DefaultOk);

  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_impact1.wav"), SoundType::ProjectileImpact);
  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_impact2.wav"), SoundType::ProjectileImpact);
  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_impact3.wav"), SoundType::ProjectileImpact);

  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_launch1.wav"), SoundType::ProjectileLaunch);
  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_launch2.wav"), SoundType::ProjectileLaunch);
  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_launch3.wav"), SoundType::ProjectileLaunch);
  this->load(AM::full_name("sounds/drow/Drow_Ranger_projectile_launch4.wav"), SoundType::ProjectileLaunch);

  this->load(AM::full_name("sounds/items/Blink_start_(AM).wav"), SoundType::BlinkStart);

  this->load(AM::full_name("sounds/invoker/EMP_target.wav"), SoundType::EmpStart);
  this->load(AM::full_name("sounds/invoker/EMP_effect.wav"), SoundType::EmpExplode);

  this->load(AM::full_name("sounds/invoker/Alacrity.wav"), SoundType::Concentrate);
  this->load(AM::full_name("sounds/sand_king/Burrowstrike.wav"), SoundType::Dash);
}

void SoundsHandler::load(const std::string& filename, const SoundType& type)
{
  this->buffers[type].push_back({});
  sf::SoundBuffer& buffer = this->buffers[type].back();
  if (!buffer.loadFromFile(filename))
    {
      std::string error = "Failed to load sound "s + filename;
      throw std::runtime_error(error.data());
    }
}

void SoundsHandler::play(const SoundType& type, const bool limit, const float volume)
{
  if (limit && std::chrono::steady_clock::now() < this->timeouts[type])
    return;
  auto buffer_it = this->buffers.find(type);
  if (buffer_it == this->buffers.end())
    throw std::runtime_error("No loaded sound of that type");
  std::uniform_int_distribution<std::size_t> uni_dist(0, buffer_it->second.size() - 1);
  log_debug("Choosing a number between 0 and " << buffer_it->second.size() - 1);
  std::size_t index = uni_dist(this->random_engine);
  log_debug("chose: " << index);
  sf::SoundBuffer& buffer = buffer_it->second[index];
  auto sound_it = std::find_if(this->sounds.begin(),
                               this->sounds.end(),
                               [](const Sound& sound)
                               {
                                 return !sound.is_playing();
                               });
  if (sound_it == this->sounds.end())
    {
      log_warning("Too many sounds already playing, ignoring that sound");
      return ;
    }
  sound_it->play(buffer, volume);
  if (limit)
    {
      this->timeouts[type] = std::chrono::steady_clock::now() + 500ms +
        std::chrono::microseconds(buffer.getDuration().asMicroseconds());
    }
}
