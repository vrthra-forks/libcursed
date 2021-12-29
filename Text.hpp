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

#ifndef LIBCURSED__TEXT_HPP__
#define LIBCURSED__TEXT_HPP__

#include <string>
#include <vector>

#include "guts/WindowWidget.hpp"
#include "ColorTree.hpp"

namespace cursed {

// A scrollable widget for displaying static text.
class Text : public guts::WindowWidget
{
public:
    // Constructs an empty text widget.  Can throw `std::runtime_error`.
    explicit Text();

    Text(const Text &rhs) = delete;
    Text(Text &&rhs) = delete;
    Text & operator=(const Text &rhs) = delete;
    Text & operator=(Text &&rhs) = delete;

public:
    // Retrieves height.
    int getHeight() const;

    // Assigns list of lines.
    void setLines(std::vector<ColorTree> newLines);

    // Scrolls all the way up.
    void scrollToTop();
    // Scrolls all the way down.
    void scrollToBottom();

    // Scrolls text one line down.
    void scrollDown();
    // Scrolls text one line up.
    void scrollUp();

private:
    // Updates state of this widget to be published on the screen.
    virtual void draw() override;

    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() override;
    // Retrieves horizontal size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredWidth() override;

    // Notifies widget of new position and size.
    virtual void placed(guts::Pos newPos, guts::Size newSize) override;

private:
    std::vector<ColorTree> lines; // Text itself.
    int top;                      // First element to display.
    int height;                   // Screen height.
};

}

#endif // LIBCURSED__TEXT_HPP__
