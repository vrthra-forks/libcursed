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

#ifndef LIBCURSED__GUTS__WINDOWWIDGET_HPP__
#define LIBCURSED__GUTS__WINDOWWIDGET_HPP__

#include <utility>

#include "Pos.hpp"
#include "Size.hpp"
#include "Widget.hpp"
#include "Window.hpp"

namespace cursed { namespace guts {

// Base class for all Window-based widgets in the library.
class WindowWidget : public Widget
{
public:
    // Sets background format of the window (it affects more than background
    // with attributes and/or foreground).
    void setBackground(Format format);

protected:
    // Resizes and moves window.
    virtual void placed(Pos newPos, Size newSize) override;

protected:
    Window win; // Window object.
};

inline void
WindowWidget::setBackground(Format format)
{ win.setBackground(std::move(format)); }

inline void
WindowWidget::placed(Pos newPos, Size newSize)
{ win.place(newPos, newSize); }

} }

#endif // LIBCURSED__GUTS__WINDOWWIDGET_HPP__
