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

#ifndef LIBCURSED__LIST_HPP__
#define LIBCURSED__LIST_HPP__

#include <string>
#include <vector>

#include "ColorTree.hpp"
#include "WindowWidget.hpp"

namespace cursed {

// A scrollable list of textual items.
class List : public WindowWidget
{
public:
    // Constructs an empty list.  Can throw `std::runtime_error`.
    explicit List();

    List(const List &rhs) = delete;
    List(List &&rhs) = delete;
    List & operator=(const List &rhs) = delete;
    List & operator=(List &&rhs) = delete;

public:
    // Assigns list of items.
    void setItems(std::vector<ColorTree> newItems);

    // Updates state of this widget to be published on the screen.
    virtual void draw() override;

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

    // Returns value of the element under the cursor or an empty string for
    // empty list.
    std::wstring getCurrent() const;

private:
    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() override;

    // Notifies widget of new position and size.
    virtual void placed(Pos newPos, Size newSize) override;

private:
    std::vector<ColorTree> items; // List of items.
    int pos;                      // Current cursor position.
    int top;                      // First element to display.
    int height;                   // Screen height.
    Format itemHi;                // Visual style of an item.
    Format currentHi;             // Visual style of the current item.
};

}

#endif // LIBCURSED__LIST_HPP__
