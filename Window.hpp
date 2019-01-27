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

#ifndef LIBCURSED__WINDOW_HPP__
#define LIBCURSED__WINDOW_HPP__

#include <cwctype>

namespace cursed {

class ColorTree;
class Pos;
class Size;

namespace guts {

// Text attributes.
enum class Attribs
{
    Reversed // Reverse background and foreground colors.
};

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

    // Updates size and position.
    void place(Pos newPos, Size newSize);

    // Prints colored text on the window at the current cursor position.
    void print(const ColorTree &colored);

private:
    void *ptr; // Opaque pointer to the resource.
};

// Clears window.
void (werase)(Window &win);
// Publishes changes to be displayed on the next screen update.
void (wnoutrefresh)(Window &win);

// Sets time to wait for input on a window (negative means block).
void (wtimeout)(Window &win, int delay);
// Reads input on a window.
int (wget_wch)(Window &win, std::wint_t *wch);

// Enables an attribute.
void (wattron)(Window &win, Attribs attrs);
// Disables an attribute.
void (wattroff)(Window &win, Attribs attrs);
// Sets cursor position inside the window.
void (wmove)(Window &win, int y, int x);
// Erases current line until its end.
void (wclrtoeol)(Window &win);
// Prints string on the window at current cursor position.
void (wprintw)(Window &win, const char fmt[], ...)
    __attribute__ ((format(printf, 2, 3)));
// Prints string on the window at specified cursor position.
void (mvwprintw)(Window &win, int y, int x, const char fmt[], ...)
    __attribute__ ((format(printf, 4, 5)));

}

}

#endif // LIBCURSED__WINDOW_HPP__
