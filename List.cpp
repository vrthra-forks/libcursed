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

#include "List.hpp"

#include <curses.h>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Size.hpp"

using namespace cursed;

List::List() : win(nullptr, &delwin), pos(0), top(0), height(0)
{
    win.reset(newwin(0, 0, 0, 0));
    if (win == nullptr) {
        throw std::runtime_error("Failed to create window for a list");
    }
}

void
List::setItems(std::vector<std::wstring> newItems)
{
    items = std::move(newItems);
    top = 0;
    pos = 0;
}

void
List::draw()
{
    if (items.empty()) {
        top = 0;
        pos = 0;
        return;
    }

    if (pos < top) {
        top = pos;
    } else if (pos >= top + height) {
        top = pos - (height - 1);
    }

    werase(win.get());
    int line = 0;
    for (int i = top; i < top + height; ++i) {
        if (i == static_cast<int>(items.size())) {
            break;
        }

        const std::wstring &item = items[i];

        if (i == pos) {
            wattron(win.get(), A_REVERSE);
        }
        wmove(win.get(), line, 0);
        wclrtoeol(win.get());
        wprintw(win.get(), "%ls", item.c_str());
        if (i == pos) {
            wattroff(win.get(), A_REVERSE);
        }

        if (++line == height) {
            break;
        }
    }
    wrefresh(win.get());
}

void
List::moveToFirst()
{
    pos = 0;
}

void
List::moveToLast()
{
    pos = static_cast<int>(items.size()) - 1;
}

void
List::moveDown()
{
    if (pos < static_cast<int>(items.size()) - 1) {
        ++pos;
    }
}

void
List::moveUp()
{
    if (pos > 0) {
        --pos;
    }
}

std::wstring
List::getCurrent() const
{
    return (items.empty() ? std::wstring() : items[pos]);
}

int
List::desiredHeight()
{
    return -1;
}

void
List::placed(Pos newPos, Size newSize)
{
    wresize(win.get(), newSize.lines, newSize.cols);
    mvwin(win.get(), newPos.y, newPos.x);
    height = newSize.lines;
}
