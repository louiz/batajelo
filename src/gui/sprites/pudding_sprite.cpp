#include <gui/sprites/pudding_sprite.hpp>
#include <logging/logging.hpp>

PuddingSprite::PuddingSprite(const Unit* const unit):
  UnitSprite(unit)
{

  if (PuddingSprite::init == false)
    {
      if (!PuddingSprite::body_texture.loadFromFile("data/images/units/breton_face.png"))
        exit(1);
      PuddingSprite::init = true;
    }
  this->boinc = 1 + (0.08 * random() / RAND_MAX);
  this->boincing = 0.000001 + (0.000001 * random() / RAND_MAX);
}

void PuddingSprite::draw(Camera* camera, const ClientWorld* world, const Screen* screen) const
{
  // if (!this->is_in_screen(camera, this->building))
  //   return;
  const sf::Vector2u entpos = camera->world_to_camera_position(this->unit->pos);
  const uint x = entpos.x - camera->x;
  const uint y = entpos.y - camera->y;

  // sf::RectangleShape rectangle(sf::Vector2f(30, 30));
  // rectangle.setOutlineColor(sf::Color::Blue);
  // rectangle.setOutlineThickness(1);
  sf::Sprite sprite(PuddingSprite::body_texture);

  sprite.setScale(1, this->boinc);
  sprite.setPosition(x - this->unit->width/2, y - this->unit->width - (this->boinc*64));
  camera->draw(sprite);
                    // this->draw_unit(entity, entpos.x - this->x, entpos.y - this->y,
                    //               this->mouse_selection.contains(mouse_pos,
                    //                                              entpos, entity->width + 4) ||
                    //               this->screen->is_entity_hovered(entity),
                    //               rectangle);

}

void PuddingSprite::tick()
{
  if (this->boinc > 1.08 || this->boinc < 1)
    this->boincing = -this->boincing;
  this->boinc += this->boincing;
  // log_error("boincing: " << this->boincing << ":" << this->boinc);
}

bool PuddingSprite::init = false;
sf::Texture PuddingSprite::body_texture;
