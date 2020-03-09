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

#ifndef LIBCURSED__GUTS__WIDGET_HPP__
#define LIBCURSED__GUTS__WIDGET_HPP__

#include "Pos.hpp"
#include "Size.hpp"

namespace cursed { namespace guts {

// Base class for all widgets in the library.
class Widget
{
protected:
    // No base class destruction.
    ~Widget() = default;

public:
    // Performs position and size update.  The default just calls
    // `placed(newPos, newSize)`.
    virtual void place(Pos newPos, Size newSize);
    // Updates state of this widget to be published on the screen.
    virtual void draw() = 0;

    // Retrieves vertical size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredHeight() = 0;
    // Retrieves horizontal size policy.
    // Positive number or zero means exactly that much.
    // Negative number means at least that much in magnitude.
    virtual int desiredWidth() = 0;

private:
    // Notifies widget of new position and size.
    virtual void placed(Pos newPos, Size newSize);
};

inline void
Widget::place(Pos newPos, Size newSize)
{ placed(newPos, newSize); }

inline void
Widget::placed(Pos /*newPos*/, Size /*newSize*/)
{ }

} }

#endif // LIBCURSED__GUTS__WIDGET_HPP__
