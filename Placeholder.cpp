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

#include "Placeholder.hpp"

#include <limits>

#include "guts/Pos.hpp"
#include "guts/Size.hpp"

using namespace cursed;

Placeholder::Placeholder() : widget(nullptr)
{ }

void
Placeholder::fill(guts::Widget *filling)
{
    widget = filling;
}

void
Placeholder::draw()
{
    if (widget != nullptr) {
        widget->draw();
    }
}

int
Placeholder::desiredHeight()
{
    if (widget != nullptr) {
        return widget->getDesiredHeight();
    }
    return std::numeric_limits<int>::min();
}

int
Placeholder::desiredWidth()
{
    if (widget != nullptr) {
        return widget->getDesiredWidth();
    }
    return std::numeric_limits<int>::min();
}

void
Placeholder::placed(guts::Pos newPos, guts::Size newSize)
{
    if (widget != nullptr) {
        widget->place(newPos, newSize);
    }
}
