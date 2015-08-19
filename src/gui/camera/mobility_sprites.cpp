#include <gui/camera/mobility_sprites.hpp>
#include <assets/assets_manager.hpp>

sf::Texture MobilitySprites::texture;
sf::Sprite MobilitySprites::flag_sprite;


void MobilitySprites::init()
{
  MobilitySprites::texture.loadFromFile(AssetsManager::full_name("images/game.png"));
  MobilitySprites::texture.setSmooth(true);
  MobilitySprites::flag_sprite.setTexture(MobilitySprites::texture);
  MobilitySprites::flag_sprite.setTextureRect({{396, 260}, {128, 250}});
  MobilitySprites::flag_sprite.setOrigin(20, 240);
  MobilitySprites::flag_sprite.setScale(0.3, 0.3);
}

void MobilitySprites::insert_flag_sprites(const Path& path,
                                          std::vector<std::unique_ptr<SimpleWorldSprite>>& flags)
{
  for (const Position& pos: path)
    {
      auto sprite = std::make_unique<SimpleWorldSprite>(pos, MobilitySprites::flag_sprite);
      flags.push_back(std::move(sprite));
    }
}
