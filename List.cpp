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

#include "guts/Size.hpp"

using namespace cursed;
using namespace cursed::guts;

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

List::List() : height(0)
{
    currentHi.setReversed(true);
    currentHi.setForeground(Color::Yellow);
}

void
List::setItems(std::vector<ColorTree> newItems)
{
    items = std::move(newItems);
    ListLike::reset();
}

void
List::setItem(int pos, ColorTree newValue)
{
    items[pos] = std::move(newValue);
}

std::wstring
List::getCurrent() const
{
    if (items.empty()) {
        return std::wstring();
    }

    std::wstring current;
    items[getPos()].visit([&current](const std::wstring &text,
                                     const Format &/*format*/) {
        current += text;
    });
    return current;
}

void
List::draw()
{
    if (items.empty()) {
        ListLike::reset();

        win.erase();
        wnoutrefresh(win);
        return;
    }

    int lineNumWidth = countWidth(items.size());

    win.erase();
    int line = 0;
    int top = getTop();
    int pos = getPos();
    for (int i = top; i < top + height; ++i, ++line) {
        if (i == static_cast<int>(items.size())) {
            break;
        }

        wmove(win, line, 0);
        wclrtoeol(win);

        std::wostringstream oss;
        oss << L' '
            << std::setw(lineNumWidth) << std::to_wstring(i + 1) << L": ";

        Format hi = itemHi;
        if (i == pos) {
            hi += currentHi;
        }
        win.print(hi(oss.str() + ColorTree(items[i]) + L" "));
    }
    wnoutrefresh(win);
}

int
List::desiredHeight()
{
    return -1;
}

void
List::placed(Pos newPos, Size newSize)
{
    WindowWidget::placed(newPos, newSize);
    height = newSize.lines;
}

int
List::getSize() const
{
    return items.size();
}

int
List::getHeight() const
{
    return height;
}
