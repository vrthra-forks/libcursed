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

#ifndef LIBCURSED__LABEL_HPP__
#define LIBCURSED__LABEL_HPP__

#include "ColorTree.hpp"
#include "WindowWidget.hpp"

namespace cursed {

// Static text field.
class Label : public WindowWidget
{
public:
    // Constructs a label.  Can throw `std::runtime_error`.
    explicit Label(ColorTree initText = {});

    Label(const Label &rhs) = delete;
    Label(Label &&rhs) = delete;
    Label & operator=(const Label &rhs) = delete;
    Label & operator=(Label &&rhs) = delete;

public:
    // Updates text of the label.
    void setText(ColorTree newText);

private:
    // Updates state of this widget to be published on the screen.
    virtual void draw() override;

    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() override;

private:
    ColorTree text;   // Text of the label.
};

}

#endif // LIBCURSED__LABEL_HPP__
