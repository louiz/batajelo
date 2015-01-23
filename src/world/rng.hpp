#ifndef RNG_HPP_INCLUDED
#define RNG_HPP_INCLUDED

#include <random>

/**
 * Class to use to get every pseudo random number for the world.  For each
 * game, each participant must seed it with the exact same seed.  The seed
 * must be generated by the server (or if the game is loaded from a replay,
 * it must be read from that replay) and communicated to each client upon
 * connexion.
 */

class Rng
{
public:
  Rng();
  ~Rng() = default;
  /**
   * Seed it with a value that we got from a server or a replay.
   */
  void seed(unsigned int s);
  /**
   * Seed it with a real random number.
   */
  void seed();
  unsigned long get_seed() const;

private:
  unsigned int seed_value;
  std::mt19937 engine;

  Rng(const Rng&) = delete;
  Rng(Rng&&) = delete;
  Rng& operator=(const Rng&) = delete;
  Rng& operator=(Rng&&) = delete;
};


#endif /* RNG_HPP_INCLUDED */
