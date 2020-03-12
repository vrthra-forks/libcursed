// libcursed -- C++ classes for dealing with curses
// Copyright (C) 2019 xaizek <xaizek@posteo.net>
//
// This file is part of libcursed.
//
// libcursed is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libcursed is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libcursed.  If not, see <https://www.gnu.org/licenses/>.

#ifndef LIBCURSED__GUTS__WINDOW_HPP__
#define LIBCURSED__GUTS__WINDOW_HPP__

#include <cwctype>

#include "../ColorTree.hpp"

namespace cursed { namespace guts {

class Pos;
class Size;

// Manages window resource.
class Window
{
public:
    // Creates a window or throws `std::runtime_error`.
    Window();
    // Deletes the resource.
    ~Window();

    Window(const Window &rhs) = delete;
    Window(Window &&rhs) = delete;
    Window & operator=(const Window &rhs) = delete;
    Window & operator=(Window &&rhs) = delete;

public:
    // Retrieves window resource.
    void * raw()
    { return ptr; }

    // Updates size and position.  Throws `std::runtime_error` on failure.
    void place(Pos newPos, Size newSize);

    // Sets background format of the window (it affects more than background
    // with attributes and/or foreground).
    void setBackground(Format format);

    // Clears window.
    void erase();

    // Prints colored text on the window at the current cursor position.
    void print(const ColorTree &colored);

    // Checks whether this window is hidden and shouldn't be drawn.
    bool isHidden() const;

private:
    void *ptr;   // Opaque pointer to the resource.
    Format bg;   // Background/default format of the window.
    bool hidden; // Whether it's a hidden (resized to zero area).
};

// Sets a window flag that defines whether functional keys are recognized as
// such.  Disabled by default.
void (keypad)(Window &win, bool bf);

// Publishes changes to be displayed on the next screen update.
void (wnoutrefresh)(Window &win);

// Sets time to wait for input on a window (negative means block).
void (wtimeout)(Window &win, int delay);
// Reads input on a window.
int (wget_wch)(Window &win, std::wint_t *wch);

// Sets cursor position inside the window.
void (wmove)(Window &win, int y, int x);
// Erases current line until its end.
void (wclrtoeol)(Window &win);

} }

#endif // LIBCURSED__GUTS__WINDOW_HPP__
