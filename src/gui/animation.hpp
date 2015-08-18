#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

/**
 * Given the number of frames of an animation, and the frame duration: keeps
 * track of the current frame we should display on-screen.
 */

#include <utils/time.hpp>

class Animation
{
public:
  Animation(const std::size_t number_of_frames, const utils::Duration& frame_duration):
    number_of_frames(number_of_frames),
    frame_duration(frame_duration),
    time(0s),
    frame(0)
  {
  }
  ~Animation() = default;
  void update(const utils::Duration& dt);
  std::size_t get_current_frame() const;
  void reset();

private:
  const std::size_t number_of_frames;
  utils::Duration frame_duration;

  utils::Duration time;
  std::size_t frame;

  Animation(const Animation&) = delete;
  Animation(Animation&&) = delete;
  Animation& operator=(const Animation&) = delete;
  Animation& operator=(Animation&&) = delete;
};


#endif /* ANIMATION_HPP_INCLUDED */
