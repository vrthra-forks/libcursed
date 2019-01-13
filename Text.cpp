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

// Splits string into lines.
template <typename C>
std::vector<std::basic_string<C>>
split(const std::basic_string<C> &str)
{
    using size_type = typename std::basic_string<C>::size_type;

    std::vector<std::basic_string<C>> result;
    for (size_type pos = 0U, next = 0U;
         next != std::basic_string<C>::npos;
         pos = next + 1U) {
        next = str.find(static_cast<C>('\n'), pos);
        result.emplace_back(str, pos, next - pos);
    }
    return result;
}

Text::Text(const std::wstring &text) : lines(split(text)), top(0), height(0)
{ }

void
Text::draw()
{
    werase(win);
    int line = 0;
    for (int i = top; i < top + height; ++i, ++line) {
        if (i == static_cast<int>(lines.size())) {
            break;
        }

        wmove(win, line, 0);
        wclrtoeol(win);
        wprintw(win, " %ls ", lines[i].c_str());
    }
    wnoutrefresh(win);
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

int
Text::desiredHeight()
{
    return -1;
}

void
Text::placed(Pos newPos, Size newSize)
{
    win.place(newPos, newSize);
    height = newSize.lines;
}
