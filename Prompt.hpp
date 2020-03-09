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

#ifndef LIBCURSED__PROMPT_HPP__
#define LIBCURSED__PROMPT_HPP__

#include "guts/WindowWidget.hpp"
#include "ColorTree.hpp"

namespace cursed {

// Prompt for requesting input.
class Prompt : public guts::WindowWidget
{
public:
    // Constructs a prompt.  Can throw `std::runtime_error`.
    explicit Prompt();

    Prompt(const Prompt &rhs) = delete;
    Prompt(Prompt &&rhs) = delete;
    Prompt & operator=(const Prompt &rhs) = delete;
    Prompt & operator=(Prompt &&rhs) = delete;

public:
    // Updates text and cursor position of the prompt.
    void setText(ColorTree newText, int newPos);

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

private:
    ColorTree text; // Text of the prompt.
    int pos;        // Cursor position.
};

}

#endif // LIBCURSED__PROMPT_HPP__
