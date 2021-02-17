// libcursed -- C++ classes for dealing with curses
// Copyright (C) 2021 xaizek <xaizek@posteo.net>
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

#include "Widget.hpp"

#include "Pos.hpp"
#include "Size.hpp"

using cursed::guts::Widget;

Widget::Widget() : hasFixedSize(false)
{ }

void
Widget::setFixedSize(int colsNum, int linesNum)
{
    hasFixedSize = true;
    cols = colsNum;
    lines = linesNum;
}

int
Widget::getDesiredHeight()
{ return (hasFixedSize ? lines : desiredHeight()); }

int
Widget::getDesiredWidth()
{ return (hasFixedSize ? cols : desiredWidth()); }

void
Widget::place(Pos newPos, Size newSize)
{ placed(newPos, newSize); }

void
Widget::placed(Pos /*newPos*/, Size /*newSize*/)
{ }
