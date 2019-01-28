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

#include <cassert>

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Size.hpp"

using namespace cursed;

// Computes number of digits in a positive number (including zero).
static inline int
countWidth(int n)
{
    assert(n >= 0 && "Argument must be >= 0.");

    int width = 0;
    do {
        n /= 10;
        ++width;
    } while (n > 0);
    return width;
}

List::List() : pos(0), top(0), height(0)
{
    currentHi.setReversed(true);
}

void
List::setItems(std::vector<ColorTree> newItems)
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

        win.erase();
        wnoutrefresh(win);
        return;
    }

    if (pos < top) {
        top = pos;
    } else if (pos >= top + height) {
        top = pos - (height - 1);
    }

    int lineNumWidth = countWidth(items.size());

    win.erase();
    int line = 0;
    for (int i = top; i < top + height; ++i, ++line) {
        if (i == static_cast<int>(items.size())) {
            break;
        }

        wmove(win, line, 0);
        wclrtoeol(win);

        std::wostringstream oss;
        oss << L' '
            << std::setw(lineNumWidth) << i + 1 << L": ";

        Format primeFormat(i == pos ? currentHi : cursed::Format());
        ColorTree colored = primeFormat(oss.str() + ColorTree(items[i]) + L" ");
        win.print(itemHi(std::move(colored)));
    }
    wnoutrefresh(win);
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
List::moveDown(int by)
{
    assert(by > 0 && "Argument must be > 0.");
    pos += by;
    if (pos > static_cast<int>(items.size()) - 1) {
        pos = static_cast<int>(items.size()) - 1;
    }
}

void
List::moveUp(int by)
{
    assert(by > 0 && "Argument must be > 0.");
    pos -= by;
    if (pos < 0) {
        pos = 0;
    }
}

void
List::moveToPos(int newPos)
{
    --newPos;
    if (newPos < 0) {
        newPos = 0;
    }
    if (newPos >= static_cast<int>(items.size()) - 1) {
        newPos = items.size() - 1;
    }
    pos = newPos;
}

std::wstring
List::getCurrent() const
{
    if (items.empty()) {
        return std::wstring();
    }

    std::wstring current;
    items[pos].visit([&current](const std::wstring &text,
                                const Format &/*format*/) {
        current += text;
    });
    return current;
}

int
List::desiredHeight()
{
    return -1;
}

void
List::placed(Pos newPos, Size newSize)
{
    win.place(newPos, newSize);
    height = newSize.lines;
}
