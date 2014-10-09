#include <gui/sprites/pic_sprite.hpp>
#include <logging/logging.hpp>

PicpicSprite::PicpicSprite(const Unit* const unit):
  UnitSprite(unit),
  float_direction(-0.0002)
{
  if (PicpicSprite::init == false)
    {
      if (!PicpicSprite::body_texture.loadFromFile("data/images/units/picpic.svg.png"))
        exit(1);
      if (!PicpicSprite::eye_texture.loadFromFile("data/images/units/picpic_eye.svg.png"))
        exit(1);
      PicpicSprite::init = true;
    }
  this->height = 1 + (5 * random() / RAND_MAX);
}

void PicpicSprite::draw(Camera* camera, const ClientWorld* world, const Screen* screen) const
{
  this->draw_shadow(camera, world, screen);
  const sf::Vector2u entpos = camera->world_to_camera_position(this->unit->pos);

  const uint x = entpos.x - camera->x;
  const uint y = entpos.y - camera->y;

  sf::Sprite sprite(PicpicSprite::body_texture);
  const sf::Vector2u size = PicpicSprite::body_texture.getSize();
  sprite.setPosition(x - size.x/2, y - size.y - this->height);
  camera->draw(sprite);

  sf::Sprite eye_sprite(PicpicSprite::eye_texture);
  const sf::Vector2u eye_size = PicpicSprite::eye_texture.getSize();
  eye_sprite.setPosition(x - 10 - eye_size.x/2, y - size.y / 5 * 3 - this->height - eye_size.y/2 + 5);
  sf::Vector2i mouse_pos = camera->get_mouse_position();
  camera->draw(eye_sprite);
  eye_sprite.setPosition(x + 10 - eye_size.x/2, y - size.y / 5 * 3 - this->height - eye_size.y/2 + 5);
  camera->draw(eye_sprite);
}

void PicpicSprite::tick()
{
  if (this->height > 6 || this->height < 1)
    this->float_direction = -this->float_direction;
  this->height += this->float_direction;
}

bool PicpicSprite::init = false;
sf::Texture PicpicSprite::body_texture;
sf::Texture PicpicSprite::eye_texture;
