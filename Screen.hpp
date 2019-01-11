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

#ifndef LIBCURSED__SCREEN_HPP__
#define LIBCURSED__SCREEN_HPP__

namespace cursed {

class Widget;

// Provides information about the whole screen.
class Screen
{
public:
    // Sets widget that takes up the whole screen.
    void setMainWidget(Widget *w)
    { mainWidget = w; }

    // Handles screen resizing.
    void resize();

    // Draws main widget and makes screen updates visible on physical screen.
    void draw();

private:
    Widget *mainWidget = nullptr;
};

}

#endif // LIBCURSED__SCREEN_HPP__
