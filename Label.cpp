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

#include "Label.hpp"

#include <curses.h>

#include <string>
#include <utility>

using namespace cursed;

Label::Label(std::wstring initText)
    : win(nullptr, &delwin), text(std::move(initText))
{
    win.reset(newwin(0, 0, 0, 0));
    if (win == nullptr) {
        throw std::runtime_error("Failed to create window for a label");
    }
}

void
Label::setText(std::wstring newText)
{
    text = std::move(newText);
}

void
Label::draw()
{
    werase(win.get());
    wmove(win.get(), 0, 0);
    wprintw(win.get(), "%ls", text.c_str());
    wrefresh(win.get());
}

int
Label::desiredHeight()
{
    return 1;
}

void
Label::placed(Pos newPos, Size newSize)
{
    wresize(win.get(), newSize.lines, newSize.cols);
    mvwin(win.get(), newPos.y, newPos.x);
}
