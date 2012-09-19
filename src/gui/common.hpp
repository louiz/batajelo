/** @addtogroup Ui
 *  @{
 */

/**
 * A serie of functions calling SFML-Graphics functions, checking their
 * return value and throwing an exception in case of an error.  It makes it
 * easy to call these function in various places where we initiate the
 * graphical stuffs (for example loading images from files, creating a
 * RenderSurface, etc) and just using a catch statement to display an error
 * and cancelling the loading process.  This avoids a lot of “if
 * (sf::blabla::loadFromFile() == -1) { return -1; } and makes it possible
 * to do all that in constructors, which is cool as well.

 * OR NOT
 */

#include <exception>

struct GraphInitError: public std::exception
{

};
