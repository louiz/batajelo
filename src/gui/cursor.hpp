#ifndef __CURSOR_HPP__
# define __CURSOR_HPP__

namespace cursor
{
  enum type
    {
      Normal,
      Select,
      Move,
      Aim,
      Cast,
      // Just keep track of the possible number of cursors
      size
    };

  static const char filenames[size][20] =
  {
    "gui_cursor.png",
    "gui_cursor.png",
    "move_cursor.png",
    "gui_cursor.png",
    "gui_cursor.png"
  };
}

#endif // __CURSOR_HPP__
