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

#include "Screen.hpp"

#include <curses.h>

#include "ColorManager.hpp"
#include "Size.hpp"
#include "Widget.hpp"

using namespace cursed;

// Retrieves size of the screen.
static inline Size
getSize()
{
    Size size;
    getmaxyx(stdscr, size.lines, size.cols);
    return size;
}

void
Screen::setMainWidget(Widget *w)
{
    mainWidget = w;
    resize();
}

void
Screen::resize()
{
    if (mainWidget != nullptr) {
        mainWidget->place(Pos(), getSize());
    }
}

void
Screen::draw()
{
    if (mainWidget != nullptr) {
        guts::ColorManager::get().reset();
        mainWidget->draw();
        doupdate();
    }
}

void
Screen::showCursor()
{
    curs_set(1);
}

void
Screen::hideCursor()
{
    curs_set(0);
}

void
Screen::stopTUI()
{
    endwin();
}
