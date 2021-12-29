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

#include "Text.hpp"

#include <string>
#include <utility>
#include <vector>

using namespace cursed;
using namespace cursed::guts;

Text::Text() : top(0), height(0)
{ }

int
Text::getHeight() const
{ return height; }

void
Text::setLines(std::vector<ColorTree> newLines)
{
    lines = std::move(newLines);
    scrollToTop();
}

void
Text::scrollToTop()
{
    top = 0;
}

void
Text::scrollToBottom()
{
    top = static_cast<int>(lines.size()) - height;
    if (top < 0) {
        top = 0;
    }
}

void
Text::scrollDown()
{
    ++top;
    if (top > static_cast<int>(lines.size()) - height) {
        top = static_cast<int>(lines.size()) - height;
        if (top < 0) {
            top = 0;
        }
    }
}

void
Text::scrollUp()
{
    --top;
    if (top < 0) {
        top = 0;
    }
}

void
Text::draw()
{
    win.erase();
    int line = 0;
    for (int i = top; i < top + height; ++i, ++line) {
        if (i == static_cast<int>(lines.size())) {
            break;
        }

        wmove(win, line, 0);
        wclrtoeol(win);
        wmove(win, line, 1);
        win.print(lines[i]);
    }
    wnoutrefresh(win);
}

int
Text::desiredHeight()
{
    return -1;
}

int
Text::desiredWidth()
{
    return -1;
}

void
Text::placed(Pos newPos, Size newSize)
{
    WindowWidget::placed(newPos, newSize);
    height = newSize.lines;
}
