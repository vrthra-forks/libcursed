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

#include "guts/WindowWidget.hpp"
#include "ColorTree.hpp"
#include "ListLike.hpp"

namespace cursed {

// A scrollable list of textual items.
class List : public guts::WindowWidget, public ListLike
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
    // Updates an item at position (should be a valid index).
    void setItem(int pos, ColorTree newValue);

    // Returns value of the element under the cursor or an empty string for
    // empty list.
    std::wstring getCurrent() const;

private:
    // Updates state of this widget to be published on the screen.
    virtual void draw() override;

    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() override;

    // Notifies widget of new position and size.
    virtual void placed(guts::Pos newPos, guts::Size newSize) override;

    // Retrieves number of elements in the list.
    virtual int getSize() const override;

private:
    std::vector<ColorTree> items; // List of items.
    int height;                   // Screen height.
    Format itemHi;                // Visual style of an item.
    Format currentHi;             // Visual style of the current item.
};

}

#endif // LIBCURSED__LIST_HPP__
