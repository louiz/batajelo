#include <gui/page/page.hpp>

Page::Page(Ui* ui, sfg::Desktop* desktop, sf::RenderWindow* window):
  desktop(desktop),
  window(window),
  ui(ui)
{}
