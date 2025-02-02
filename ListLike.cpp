// libcursed -- C++ classes for dealing with curses
// Copyright (C) 2018 xaizek <xaizek@posteo.net>
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

#include "ListLike.hpp"

#include <cassert>

using namespace cursed;

ListLike::ListLike()
{
    reset();
}

void
ListLike::moveToFirst()
{
    pos = 0;
}

void
ListLike::moveToLast()
{
    pos = getSize() - 1;
}

void
ListLike::moveDown(int by)
{
    assert(by > 0 && "Argument must be > 0.");
    pos += by;
    int size = getSize();
    if (pos > size - 1) {
        pos = size - 1;
    }
}

void
ListLike::moveUp(int by)
{
    assert(by > 0 && "Argument must be > 0.");
    pos -= by;
    if (pos < 0) {
        pos = 0;
    }
}

void
ListLike::moveToPos(int newPos)
{
    int size = getSize();
    if (newPos < 0) {
        newPos = 0;
    }
    if (newPos >= size - 1) {
        newPos = size - 1;
    }
    pos = newPos;
}

void
ListLike::scrollDown()
{
    ++top;

    int size = getSize();
    int height = getHeight();
    if (top > size - height) {
        top = size - height;
        if (top < 0) {
            top = 0;
        }
    }

    if (pos < top) {
        pos = top;
    }
}

void
ListLike::scrollUp()
{
    --top;
    if (top < 0) {
        top = 0;
    }

    int height = getHeight();
    if (pos >= top + height) {
        pos = top + height - 1;
    }
}

int
ListLike::getPos() const
{
    return pos;
}

void
ListLike::reset()
{
    top = 0;
    pos = 0;
}

int
ListLike::getTop()
{
    int height = getHeight();

    if (pos < top) {
        top = pos;
    } else if (pos >= top + height) {
        top = pos - (height - 1);
    }

    if (top < 0) {
        top = 0;
    }
    return top;
}
