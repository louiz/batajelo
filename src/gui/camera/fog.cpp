#include <gui/camera/fog.hpp>
#include <gui/camera/camera.hpp>

#include <world/world.hpp>
#include <world/vision.hpp>
#include <world/location.hpp>

#include <world/map.hpp>

Fog::Fog(const unsigned int width, const unsigned int height,
         const Camera* const camera, const World* const world):
  texture(),
  sprite(),
  needs_redraw(true),
  camera(camera),
  world(world)
{
  this->resize(width, height);
}

void Fog::resize(const unsigned int width, const unsigned int height)
{
  // TODO, have a fallback
  assert(this->texture.create(width, height));
  this->texture.setSmooth(true);
  this->invalidate();
}

void Fog::invalidate()
{
  this->needs_redraw = true;
}

const sf::Sprite& Fog::get_sprite()
{
  if (this->needs_redraw)
    this->redraw();
  return this->sprite;
}

void Fog::redraw()
{
  this->texture.clear({0, 0, 0, 150});

  // For each entity (with a vision and position), draw the vision range
  for (const auto& entity: this->world->entities)
    {
      Vision* vision = entity->get<Vision>();
      Location* location = entity->get<Location>();
      if (vision && location)
        {
          sf::CircleShape circle(vision->get_range(),
                                 60);
          circle.setFillColor({0, 0, 0, 0});
          auto center = this->camera->world_to_camera_position(location->position());
          center.x -= this->camera->x;
          center.y -= this->camera->y;
          circle.setPosition(center.x, center.y);
          circle.move(-static_cast<float>(vision->get_range()),
                      -static_cast<float>(vision->get_range()) * TILE_HW_RATIO);
          circle.setScale(1, 0.666);
          this->texture.draw(circle, sf::BlendMode::BlendMultiply);
        }
    }

  this->texture.display();
  this->sprite.setTexture(this->texture.getTexture());
  this->needs_redraw = false;
}
