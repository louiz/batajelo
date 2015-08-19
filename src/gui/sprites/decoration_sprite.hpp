#ifndef DECORATION_SPRITE_HPP_INCLUDED
#define DECORATION_SPRITE_HPP_INCLUDED

/**
 * A sprite that has a world position, but is not associated with an entity.
 * Things like trees, bushes, rocks, etc.
 */

#include <gui/sprites/world_sprite.hpp>
#include <array>

template <int N>
class DecorationSprite: public WorldSprite
{
public:
  DecorationSprite(const Position& pos, const std::string& image_path, const sf::Vector2<int> size,
                   const utils::Duration& frame_duration):
    frame_duration(frame_duration),
    position(pos),
    sprite{},
    textures{},
    frame(0),
    time(0)
  {
    sf::Image image;
    if (!image.loadFromFile(image_path))
      {
        std::cout << "Failed to load image: " << image_path << std::endl;
        return;
      }
    for (auto i = 0; i < N; ++i)
      {
        if (!this->textures[i].loadFromImage(image,
                                             {{size.x*i, 0}, {size.x, size.y}}))
          {
            std::cout << "Failed to create texture from image " << image_path << std::endl;
            return;
          }
      }
    this->sprite.setTexture(this->textures[0]);
    this->sprite.setOrigin(size.x/2, size.y - 20);
  }

  ~DecorationSprite() = default;
  void tick() override final {}
  Position get_world_pos() const override final
  {
    return this->position;
  }
  void draw(sf::RenderTarget& surface, const sf::RenderStates& states) const override final
  {
    surface.draw(this->sprite, states);
  }
  void update(const utils::Duration& dt) override final
  {
    this->time += dt;
    // std::cout << "In update, time: " << time.count() << "ms" << std::endl;
    if (this->time > this->frame_duration)
      {
        while (this->time > this->frame_duration)
          {
            // std::cout << "and " << time.count() << "ms" << std::endl;
            this->time -= this->frame_duration;
            // std::cout << "now " << time.count() << "ms" << std::endl;
            // return;
            this->frame++;
            if (this->frame == N)
              this->frame = 0;
          }
        this->sprite.setTexture(this->textures[this->frame]);
      }
  }

private:
  const utils::Duration frame_duration;
  const Position position;
  sf::Sprite sprite;
  std::array<sf::Texture, N> textures;

  std::size_t frame;
  utils::Duration time;

  DecorationSprite(const DecorationSprite&) = delete;
  DecorationSprite(DecorationSprite&&) = delete;
  DecorationSprite& operator=(const DecorationSprite&) = delete;
  DecorationSprite& operator=(DecorationSprite&&) = delete;
};


#endif /* DECORATION_SPRITE_HPP_INCLUDED */
