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

#ifndef LIBCURSED__GUTS__POS_HPP__
#define LIBCURSED__GUTS__POS_HPP__

namespace cursed { namespace guts {

// Represents position on the screen in characters.
struct Pos
{
    int x; // Offset along X axis.
    int y; // Offset along Y axis.

    Pos() : x(0), y(0)
    { }
    Pos(int x, int y) : x(x), y(y)
    { }
};

} }

#endif // LIBCURSED__GUTS__POS_HPP__
