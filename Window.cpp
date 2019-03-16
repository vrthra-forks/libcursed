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

#include <stdexcept>
#include <utility>

#include "ColorManager.hpp"
#include "ColorTree.hpp"
#include "Pos.hpp"
#include "Size.hpp"

using namespace cursed::guts;
namespace guts = cursed::guts;

namespace {

// Just a combination of color pair and curses attributes.
struct Rendition
{
    int pair;
    int attrs;

    // Converts format into curses data.
    explicit Rendition(const cursed::Format &format)
    {
        attrs = 0;
        if (format.isBold()) {
            attrs |= WA_BOLD;
        }
        if (format.isReversed()) {
            attrs |= WA_REVERSE;
        }
        if (format.isUnderlined()) {
            attrs |= WA_UNDERLINE;
        }

        int fg = format.getForeground();
        int bg = format.getBackground();
        pair = ColorManager::get().makePair(fg, bg);
    }

    // Performs conversion to type known to curses.
    cchar_t toCChar() const
    {
        cchar_t cch;
        setcchar(&cch, L" ", attrs, pair, nullptr);
        return cch;
    }
};

}

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
    if (wresize(w(ptr), newSize.lines, newSize.cols) != OK) {
        throw std::runtime_error("Failed to resize curses window");
    }
    if (mvwin(w(ptr), newPos.y, newPos.x) != OK) {
        throw std::runtime_error("Failed to move curses window");
    }
}

void
Window::setBackground(Format format)
{
    bg = std::move(format);
}

void
Window::erase()
{
    Rendition rendition(bg);
    cchar_t cch = rendition.toCChar();
    wbkgrndset(w(ptr), &cch);

    werase(w(ptr));
}

void
Window::print(const ColorTree &colored)
{
    colored.visit([&](const std::wstring &text, const Format &format) {
        Rendition rendition(format);
        wattr_set(w(ptr), rendition.attrs, rendition.pair, nullptr);
        wprintw(w(ptr), "%ls", text.c_str());
    });
}


void
(guts::keypad)(Window &win, bool bf)
{
    keypad(w(win.raw()), bf);
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
(guts::wmove)(Window &win, int y, int x)
{
    wmove(w(win.raw()), y, x);
}

void
(guts::wclrtoeol)(Window &win)
{
    wclrtoeol(w(win.raw()));
}
