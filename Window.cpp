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

#include "Window.hpp"

#include <curses.h>

#include <cstdarg>

#include <stdexcept>

#include "Pos.hpp"
#include "Size.hpp"

using namespace cursed::guts;
namespace guts = cursed::guts;

// A shorthand for converting `void *` to `WINDOW *`.
static inline WINDOW *
w(void *ptr)
{
    return static_cast<WINDOW *>(ptr);
}

Window::Window()
{
    ptr = newwin(0, 0, 0, 0);
    if (ptr == nullptr) {
        throw std::runtime_error("Failed to create curses window");
    }
}

Window::~Window()
{
    delwin(w(ptr));
}

void
Window::place(Pos newPos, Size newSize)
{
    // Move fails if window would be only partially visible at destination, so
    // resizing should be performed first.
    wresize(w(ptr), newSize.lines, newSize.cols);
    mvwin(w(ptr), newPos.y, newPos.x);
}

void
(guts::werase)(Window &win)
{
    werase(w(win.raw()));
}

void
(guts::wnoutrefresh)(Window &win)
{
    wnoutrefresh(w(win.raw()));
}

void
(guts::wtimeout)(Window &win, int delay)
{
    wtimeout(w(win.raw()), delay);
}

int
(guts::wget_wch)(Window &win, std::wint_t *wch)
{
    return wget_wch(w(win.raw()), wch);
}

void
(guts::wattron)(Window &win, Attribs attrs)
{
    int cursesAttrs = 0;
    switch (attrs) {
        case Attribs::Reversed: cursesAttrs = A_REVERSE; break;
    }
    wattron(w(win.raw()), cursesAttrs);
}

void
(guts::wattroff)(Window &win, Attribs attrs)
{
    int cursesAttrs = 0;
    switch (attrs) {
        case Attribs::Reversed: cursesAttrs = A_REVERSE; break;
    }
    wattroff(w(win.raw()), cursesAttrs);
}

void
(guts::wmove)(Window &win, int y, int x)
{
    wmove(w(win.raw()), y, x);
}

void
(guts::wclrtoeol)(Window &win)
{
    wclrtoeol(w(win.raw()));
}

void
(guts::wprintw)(Window &win, const char fmt[], ...)
{
    va_list vaList;
    va_start(vaList, fmt);
    vwprintw(w(win.raw()), fmt, vaList);
    va_end(vaList);
}

void
(guts::mvwprintw)(Window &win, int y, int x, const char fmt[], ...)
{
    wmove(w(win.raw()), y, x);

    std::va_list vaList;
    va_start(vaList, fmt);
    vwprintw(w(win.raw()), fmt, vaList);
    va_end(vaList);
}
