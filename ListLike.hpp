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

#ifndef LIBCURSED__LISTLIKE_HPP__
#define LIBCURSED__LISTLIKE_HPP__

namespace cursed {

// Provides cursor and scrolling functionality common for lists.
class ListLike
{
protected:
    // Performs initialization.
    ListLike();

public:
    // Puts cursor on the first element.
    void moveToFirst();
    // Puts cursor on the last element.
    void moveToLast();

    // Moves cursor `by` elements down.
    void moveDown(int by = 1);
    // Moves cursor `by` elements up.
    void moveUp(int by = 1);

    // Moves cursor to the specified position (incorrect value is turned into
    // closes bound).
    void moveToPos(int newPos);

    // Scrolls text one line down.  If current position ends up being outside of
    // viewport, it's adjusted to be visible.
    void scrollDown();
    // Scrolls text one line up.  If current position ends up being outside of
    // viewport, it's adjusted to be visible.
    void scrollUp();

    // Retrieves current position.
    int getPos() const;

protected:
    // Resets scroll and cursor positions.
    void reset();
    // Retrieves scroll position.
    int getTop();

private:
    // Retrieves number of elements in the list.
    virtual int getSize() const = 0;

    // Retrieves viewport height.
    virtual int getHeight() const = 0;

private:
    int pos; // Current cursor position.
    int top; // Scroll position (first element to display).
};

}

#endif // LIBCURSED__LISTLIKE_HPP__
