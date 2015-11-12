#ifndef BATAJELO_APPROX_HPP
#define BATAJELO_APPROX_HPP

/**
 * Define our own approx for our own types.
 */
#include <fixmath/fix16.hpp>
#include "catch.hpp"

Approx fix_approx(const Fix16& value);
Approx fix_approx(const Fix16& value, const double epsilon);

bool operator==(const Fix16& lhs, const Approx& rhs);

#endif //BATAJELO_APPROX_HPP
