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

#include <curses.h>

#include <memory>
#include <string>
#include <vector>

namespace cursed {

// A scrollable list of textual items.
class List
{
public:
    // Constructs an empty list that takes up all screen.  Can throw
    // `std::runtime_error`.
    explicit List();

    List(const List &rhs) = delete;
    List(List &&rhs) = delete;
    List & operator=(const List &rhs) = delete;
    List & operator=(List &&rhs) = delete;

public:
    // Assigns list of items.
    void setItems(std::vector<std::wstring> newItems);

    // Performs size update.
    void resize();
    // Draws contents on the screen.
    void draw();

    // Puts cursor on the first element.
    void moveToFirst();
    // Puts cursor on the last element.
    void moveToLast();

    // Moves cursor one element down.
    void moveDown();
    // Moves cursor one element up.
    void moveUp();

    // Returns value of the element under the cursor or an empty string for
    // empty list.
    std::wstring getCurrent() const;

private:
    std::unique_ptr<WINDOW, decltype(&delwin)> win; // Window object.
    std::vector<std::wstring> items;                // List of items.
    int pos;                                        // Current cursor position.
    int top;                                        // First element to display.
    int height;                                     // Screen height.
};

}

#endif // LIBCURSED__LIST_HPP__
