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

#ifndef LIBCURSED__TRACK_HPP__
#define LIBCURSED__TRACK_HPP__

#include <vector>

#include "Widget.hpp"

namespace cursed {

// Organizes widgets it contains vertically taking their height policy into
// account.
class Track : public Widget
{
public:
    // Adds item to this track.
    void addItem(Widget *w);

public:
    // Performs position and size update.
    virtual void place(Pos newPos, Size newSize) override;
    // Draws contents on the screen.
    virtual void draw() override;

private:
    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() override;

private:
    std::vector<Widget *> widgets; // Child widgets of this track.
};

}

#endif // LIBCURSED__TRACK_HPP__
