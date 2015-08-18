#ifndef SPRITE_SHEET_HPP_INCLUDED
#define SPRITE_SHEET_HPP_INCLUDED

/**
 * Class managing a sprite sheet. This is a single image, loaded and cut as
 * an array of textures, representing (for examples) the different frames of
 * an animation.
 */

#include <SFML/Graphics.hpp>

#include <vector>
#include <cassert>

#include <logging/logging.hpp>

class SpriteSheet
{
public:
  SpriteSheet() = default;
  ~SpriteSheet() = default;
  /**
   * Takes the image of the sprite sheet to use, and the size of each
   * texture to cut in it.
   */
  void load(const std::string& image_path, const sf::Vector2<int> size)
  {
    sf::Image image;
    if (!image.loadFromFile(image_path))
      {
        log_error("Failed to load image " << image_path);
        return;
      }
    // Check if the image size is a multiple of the sub-images’ size
    auto image_size = image.getSize();
    assert(image_size.x % size.x == 0);
    this->textures.resize(image_size.x / size.x);
    for (auto i = 0u; i < this->textures.size(); ++i)
      {
        if (!this->textures[i].loadFromImage(image,
                                             {{size.x * static_cast<int>(i), 0}, {size.x, size.y}}))
          {
            log_error("Failed to create texture from image " << image_path);
            return;
          }
      }
  }

  const sf::Texture& get_texture(const std::size_t index) const
  {
    assert(index < this->textures.size());
    return this->textures[index];
  }
  std::size_t get_nb_textures() const
  {
    return this->textures.size();
  }

private:
  std::vector<sf::Texture> textures;

  SpriteSheet(const SpriteSheet&) = delete;
  SpriteSheet(SpriteSheet&&) = delete;
  SpriteSheet& operator=(const SpriteSheet&) = delete;
  SpriteSheet& operator=(SpriteSheet&&) = delete;
};


#endif /* SPRITE_SHEET_HPP_INCLUDED */
