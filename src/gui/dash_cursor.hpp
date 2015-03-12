#ifndef DASH_CURSOR_HPP_INCLUDED
#define DASH_CURSOR_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <gui/cursor.hpp>
#include <world/entity.hpp>

class GameClient;

cursor::type draw_dash_concentrate_cursor(const sf::Vector2i& mouse_position,
                                          GameClient* game);

#endif /* DASH_CURSOR_HPP_INCLUDED */
