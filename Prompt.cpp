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

#include "Prompt.hpp"

#include <utility>

using namespace cursed;

Prompt::Prompt() : pos(0)
{ }

void
Prompt::setText(ColorTree newText, int newPos)
{
    text = std::move(newText);
    pos = newPos;
}

void
Prompt::draw()
{
    win.erase();
    wmove(win, 0, 0);
    win.print(text);
    wmove(win, 0, pos);
    wnoutrefresh(win);
}

int
Prompt::desiredHeight()
{
    return 1;
}

void
Prompt::placed(Pos newPos, Size newSize)
{
    win.place(newPos, newSize);
}
