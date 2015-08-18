#include <gui/animation.hpp>

void Animation::update(const utils::Duration& dt)
{
  this->time += dt;
  while (this->time >= this->frame_duration)
    {
      this->frame++;
      if (this->frame == this->number_of_frames)
        this->frame = 0;
      this->time -= this->frame_duration;
    }
}

std::size_t Animation::get_current_frame() const
{
  return this->frame;
}

void Animation::reset()
{
  this->frame = 0;
}
